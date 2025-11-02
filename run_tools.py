import os
import shutil
import subprocess
import sys
import threading
import time
import argparse
from http.server import SimpleHTTPRequestHandler, HTTPServer

# -----------------------------------------------------------------
# --- Configuration ---
# -----------------------------------------------------------------
WASM_CRATE_NAME = "wasm_backend"
WASM_CRATE_DIR = "wasm_backend"
PKG_DIR = os.path.join(WASM_CRATE_DIR, "pkg")
WWW_DIR = "."  # Serve from the project root

BUILD_PROFILE_FLAG = "--release"
RUST_TARGET = "wasm32-unknown-emscripten"

# Paths for Emscripten Linker (Relative to WASM_CRATE_DIR)
CARGO_TARGET_DIR_REL = "target"
_JS_LIBRARY_PATH_REL = "wasm_library.js"
_RELATIVE_OUTPUT_PATH_REL = os.path.join(CARGO_TARGET_DIR_REL, RUST_TARGET, "release", f"{WASM_CRATE_NAME}.js")

WATCH_DIRS = ["src", "hal", "gui", os.path.join(WASM_CRATE_DIR, "src")] # Directories to monitor for changes
WATCH_INTERVAL = 1  # Polling interval in seconds
SERVER_PORT = 8080

# Derived Path (Absolute)
WASM_CRATE_ABS_DIR = os.path.abspath(WASM_CRATE_DIR)
WASM_OUTPUT_DIR = os.path.join(WASM_CRATE_ABS_DIR, CARGO_TARGET_DIR_REL, RUST_TARGET, "release")
WASM_FILE_BASE = os.path.join(WASM_OUTPUT_DIR, WASM_CRATE_NAME)

# Global for Watcher state
FILE_SNAPSHOT = {}
LAST_BUILD_TIME = time.time() - 3600 # Start with a time far in the past

# -----------------------------------------------------------------
# --- Build & Clean Logic ---
# -----------------------------------------------------------------

def clean_wasm_artifacts():
    """Cleans WASM build artifacts (cargo clean and removing pkg dir)."""
    print("\n--- Cleaning WASM Artifacts ---")

    # 1. Clean cargo artifacts
    if os.path.exists(WASM_CRATE_ABS_DIR):
        print(f"Running 'cargo clean' in {WASM_CRATE_ABS_DIR}")
        try:
            subprocess.run(["cargo", "clean"], check=True, cwd=WASM_CRATE_ABS_DIR, stdout=sys.stdout, stderr=sys.stderr)
        except Exception as e:
            print(f"Warning: Cargo clean failed. Ensure cargo is in PATH. {e}")

    # 2. Remove pkg directory
    pkg_abs_dir = os.path.join(os.path.abspath("."), PKG_DIR)
    if os.path.exists(pkg_abs_dir):
        print(f"Removing package directory: {pkg_abs_dir}")
        shutil.rmtree(pkg_abs_dir)

    print("WASM cleanup complete.")

def build_wasm():
    """Builds the WASM target with Emscripten linker flags using direct cargo call."""
    print(f"\n--- Building WASM target ({RUST_TARGET}) ---")

    # 1. Setup RUSTFLAGS with all Emscripten linker arguments
    rustflags = (
        f"-Clink-arg=-sENVIRONMENT=web,worker "
        f"-Clink-arg=-sFILESYSTEM=0 "
        f"-Clink-arg=-sALLOW_MEMORY_GROWTH=0 "
        f"-Clink-arg=-o "
        f"-Clink-arg={_RELATIVE_OUTPUT_PATH_REL} "
        f"-Clink-arg=--js-library "
        f"-Clink-arg={_JS_LIBRARY_PATH_REL} "
        f"-Clink-arg=--export=_lv_tick_inc "
        f"-Clink-arg=--export=_app_loop"
    )

    print(f"RUSTFLAGS: {rustflags}")

    proc_env = os.environ.copy()
    proc_env["RUSTFLAGS"] = rustflags
    proc_env["RUST_TARGET"] = RUST_TARGET

    # 2. The actual cargo command (Non-recursive, direct call)
    command = [
        "cargo",
        "build",
        f"--target={RUST_TARGET}",
        BUILD_PROFILE_FLAG
    ]

    try:
        print(f"$ {' '.join(command)} (in {WASM_CRATE_ABS_DIR})")
        subprocess.run(command, check=True, cwd=WASM_CRATE_ABS_DIR, env=proc_env, stdout=sys.stdout, stderr=sys.stderr)
    except subprocess.CalledProcessError as e:
        print(f"Error executing cargo build: {e}")
        return False
    except FileNotFoundError:
        print("Error: 'cargo' command not found. Ensure Rust toolchain and target are installed.")
        return False

    # 3. Copy Artifacts
    pkg_abs_dir = os.path.join(os.path.abspath("."), PKG_DIR)
    print(f"\nCopying Emscripten artifacts to {pkg_abs_dir}...")
    os.makedirs(pkg_abs_dir, exist_ok=True)

    try:
        # Copy the JS file
        src_js = os.path.join(WASM_CRATE_ABS_DIR, _RELATIVE_OUTPUT_PATH_REL)
        dest_js = os.path.join(pkg_abs_dir, f"{WASM_CRATE_NAME}.js")
        shutil.copy2(src_js, dest_js)
        print(f"Copied {os.path.basename(src_js)} to {dest_js}")

        # Copy the WASM file
        src_wasm = f"{WASM_FILE_BASE}.wasm"
        dest_wasm = os.path.join(pkg_abs_dir, f"{WASM_CRATE_NAME}.wasm")
        shutil.copy2(src_wasm, dest_wasm)
        print(f"Copied {os.path.basename(src_wasm)} to {dest_wasm}")

    except FileNotFoundError as e:
        print(f"\nError: Could not find build artifact to copy. Missing file: {e.filename}")
        return False

    print("WASM Build and Packaging complete.")
    return True

# -----------------------------------------------------------------
# --- Server Implementation ---
# -----------------------------------------------------------------

class CORSRequestHandler(SimpleHTTPRequestHandler):
    """A request handler that serves files, adds CORS headers, and sets WASM MIME type."""
    def end_headers(self):
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'GET, POST, OPTIONS')
        self.send_header('Access-Control-Allow-Headers', 'X-Requested-With, Content-Type')
        if self.path.endswith('.wasm'):
            self.send_header('Content-Type', 'application/wasm')
        SimpleHTTPRequestHandler.end_headers(self)

    def do_OPTIONS(self):
        self.send_response(200)
        self.end_headers()

def start_server(port):
    """Starts a simple HTTP server thread."""
    os.chdir(WWW_DIR)
    server_address = ('', port)
    httpd = HTTPServer(server_address, CORSRequestHandler)
    server_thread = threading.Thread(target=httpd.serve_forever)

    print(f"\n[SERVER] Serving at http://localhost:{port}/ (PID: {os.getpid()})")
    server_thread.start()
    return httpd

# -----------------------------------------------------------------
# --- File Watcher Implementation ---
# -----------------------------------------------------------------

def get_file_modification_time(filepath):
    """Safely returns the last modification time of a file."""
    try:
        return os.stat(filepath).st_mtime
    except FileNotFoundError:
        return 0

def get_dir_contents_recursive(directory):
    """Generates all files in a directory recursively."""
    if not os.path.isdir(directory):
        return []

    file_list = []
    for root, _, files in os.walk(directory):
        for file in files:
            # Only monitor relevant source files
            if file.endswith(('.rs', '.c', '.h', '.cpp', '.ino')):
                file_list.append(os.path.join(root, file))
    return file_list

def rebuild_and_update_time():
    """Runs the build and updates the global build time."""
    global LAST_BUILD_TIME
    if build_wasm():
        LAST_BUILD_TIME = time.time()
        print("[WATCHER] Build finished successfully.")
        return True
    return False

def watch_files(dirs):
    """Polls directories for changes and triggers rebuild."""
    global FILE_SNAPSHOT

    # Initial scan of files
    for d in dirs:
        for f in get_dir_contents_recursive(d):
            FILE_SNAPSHOT[f] = get_file_modification_time(f)

    print(f"\n[WATCHER] Monitoring the following directories: {dirs}")
    print("[WATCHER] Watching for file changes (Ctrl+C to stop)...")

    while True:
        has_changed = False
        current_snapshot = {}

        # Rescan
        for d in dirs:
            for f in get_dir_contents_recursive(d):
                current_snapshot[f] = get_file_modification_time(f)

        # Compare
        for f, mtime in current_snapshot.items():
            # Check if file is new, or if modification time is newer than the last successful build
            if f not in FILE_SNAPSHOT or mtime > FILE_SNAPSHOT[f] and mtime > LAST_BUILD_TIME:
                print(f"[WATCHER] Change detected in: {f}")
                FILE_SNAPSHOT = current_snapshot
                has_changed = True
                break

        if has_changed:
            rebuild_and_update_time()

        time.sleep(WATCH_INTERVAL)


# -----------------------------------------------------------------
# --- Main CLI Dispatcher ---
# -----------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(description="Cross-platform Rust WASM build and dev server.")
    parser.add_argument('command', choices=['build', 'clean', 'serve', 'watch', 'dev'], help="The action to perform.")
    args = parser.parse_args()

    try:
        if args.command == 'build':
            if not rebuild_and_update_time(): sys.exit(1)

        elif args.command == 'clean':
            clean_wasm_artifacts()

        elif args.command == 'serve':
            # Server only (blocking)
            httpd = start_server(SERVER_PORT)
            while True: time.sleep(1)

        elif args.command == 'watch':
            # Watcher only (blocking), runs initial build
            if not rebuild_and_update_time(): sys.exit(1)
            watch_files(WATCH_DIRS)

        elif args.command == 'dev':
            # Full Dev Loop (Server + Watcher)
            print("\n--- Starting Development Environment (Server + Watcher) ---")

            if not rebuild_and_update_time(): sys.exit(1)

            httpd = start_server(SERVER_PORT)
            watch_files(WATCH_DIRS)

    except KeyboardInterrupt:
        print("\n[CLI] Received Ctrl+C. Shutting down...")
        # Note: If server thread is running, it will be cleaned up when the main process exits.
        sys.exit(0)
    except Exception as e:
        print(f"\n[CLI] An unexpected error occurred: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
