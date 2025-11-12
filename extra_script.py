import os
import subprocess
import sys
import shutil

Import("env")

# -----------------------------------------------------------------
# --- Configuration (Used only for clean target execution) ---
# -----------------------------------------------------------------
WASM_CRATE_DIR = "wasm_backend"

# -----------------------------------------------------------------
# --- Helper to call run_tools.py for simple tasks ---
# -----------------------------------------------------------------
def call_run_tools(command_name):
    """Executes a command within run_tools.py."""
    python_script_path = os.path.join(env.subst("$PROJECT_DIR"), "run_tools.py")
    # Use sys.executable to ensure we use the correct Python interpreter
    command = [sys.executable, python_script_path, command_name]

    print(f"\n--- Executing run_tools.py command: '{command_name}' ---")
    print(f"Command: {' '.join(command)}")

    try:
        # Execute the command. Blocks until task is complete.
        subprocess.run(command, check=True, stdout=sys.stdout, stderr=sys.stderr)
    except subprocess.CalledProcessError as e:
        print(f"\nFATAL ERROR: run_tools.py '{command_name}' failed. {e}")
        return 1
    except FileNotFoundError:
        print(f"\nFATAL ERROR: Could not find Python interpreter or the script '{python_script_path}'.")
        return 1

    return 0

# -----------------------------------------------------------------
# --- WASM Clean Target ---
# -----------------------------------------------------------------
def clean_wasm_artifacts(target, source, env):
    """Cleans WASM build artifacts by calling run_tools.py clean."""
    # Note: We can no longer use PlatformIO's cargo clean inside an external script,
    # so we delegate the full cleanup to run_tools.py.
    return call_run_tools("clean")

# -----------------------------------------------------------------
# --- WASM Build Target ---
# -----------------------------------------------------------------
def build_wasm_target(target, source, env):
    """Builds the WASM target by calling run_tools.py build."""
    # All cargo logic is now in run_tools.py
    return call_run_tools("build")

# -----------------------------------------------------------------
# --- WASM Serve/Watch Target ---
# -----------------------------------------------------------------
def serve_and_watch_action(target, source, env):
    """Starts the full development loop by calling run_tools.py dev."""
    # The 'dev' command runs the initial build, server thread, and watcher loop.
    return call_run_tools("dev")


# Add custom SCons target for the WASM build: pio run -t web
env.AddCustomTarget(
    "web",
    None,
    env.Action(build_wasm_target, "Building WASM target (via run_tools.py)...")
)

# Add custom SCons target for WASM clean: pio run -t clean-wasm
env.AddCustomTarget(
    "clean-wasm",
    None,
    env.Action(clean_wasm_artifacts, "Cleaning WASM build artifacts (via run_tools.py)...")
)

# Add custom SCons target for serve/watch: pio run -t serve
env.AddCustomTarget(
    "serve",
    None,
    env.Action(serve_and_watch_action, "Building, Serving, and Watching WASM files (via run_tools.py)...")
)
