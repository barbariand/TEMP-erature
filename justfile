# -----------------------------------------------------------------
# --- Placeholders ---
# Set these to match your project structure
# -----------------------------------------------------------------
# The name of your wasm crate (from its Cargo.toml)
WASM_CRATE_NAME := "wasm_backend"

# The directory containing your wasm crate's Cargo.toml
WASM_CRATE_DIR := "wasm_backend"

# The final output directory for the JS/WASM package
PKG_DIR := "wasm_backend/pkg"

# The directory containing your index.html
WWW_DIR := "." # Assuming index.html is in the root

# --- Build Profile ---
BUILD_PROFILE := "release"

# --- Internal Paths (Derived) ---
# Set the cargo --profile flag
BUILD_PROFILE_FLAG := "--release"

CARGO_TARGET_DIR := WASM_CRATE_DIR+"/target"
RUST_TARGET := "wasm32-unknown-emscripten"
WASM_FILE_BASE := CARGO_TARGET_DIR+"/"+RUST_TARGET+"/"+BUILD_PROFILE+"/"+WASM_CRATE_NAME

_JS_LIBRARY_PATH := "wasm_library.js"
_RELATIVE_OUTPUT_PATH := "target/"+RUST_TARGET+"/"+BUILD_PROFILE+"/"+WASM_CRATE_NAME+".js"

# ---
# PlatformIO & Native Recipes
# ---
default: env-info

@env-info:
    @echo "--- Environment Check ---"
    @echo "SDL Include Path: $(SDL2_INCLUDE_PATH)"
    @echo "SDL Library Path: $(SDL2_LIBRARY_PATH)"
    @echo "Arduino Dir: $(Arduino_DIR)"
    @echo "PlatformIO Version:"
    @pio --version
    @echo "--- Nix Shell Active ---"


@clean:
    @echo "Cleaning all build artifacts for all environments..."
    pio run --target clean
    @echo "Cleaning library dependencies..."
    rm -rf .pio/libdeps
    @echo "Cleaning WASM build artifacts..."
    cd {{WASM_CRATE_DIR}} && cargo clean
    rm -rf {{PKG_DIR}}


@clangd:
    @pio run -t compiledb

@clangd-native:
    @pio run -t compiledb -e native

@native:
    @echo "Building native environment (SDL)..."
    pio run -e native
    @echo "Native build complete. Run 'just run' to execute."

@run: native
    @echo "Executing native program..."
    ./.pio/build/native/program
    @echo "Execution finished."

@build-temp:
    @echo "Building ESP32 environment (Temp)..."
    pio run -e Temp

@upload: build-temp
    @if [ ! -e /dev/ttyACM0 ]; then \
        echo "ERROR: /dev/ttyACM0 (T-Display-AMOLED) does not exist."; \
        echo "Check USB connection or port name."; \
        exit 1; \
    fi
    @echo "Uploading to /dev/ttyACM0..."
    pio run --target upload --upload-port /dev/ttyACM0 -e Temp

@monitor:
    @echo "Starting serial monitor..."
    pio device monitor -e Temp

#-----------------------------------------------------------------------------
# WEB ASSEMBLY BUILD & SERVE
#-----------------------------------------------------------------------------

## Build the WASM library and generate JS bindings.
@web:
    @echo "Building WASM target ({{RUST_TARGET}})..."

    # This is the path to the final JS file, *relative to the crate dir*

    @echo "Forcing linker output to: " {{_RELATIVE_OUTPUT_PATH}}

    # This is the path to the JS library, *relative to the crate dir*

    # Put all RUSTFLAGS on one line to avoid shell escaping issues.
    #
    # --- THIS IS THE FIX ---
    # We add two new link args:
    # 1. --js-library: This links your library.js, fixing "undefined symbol" errors.
    # 2. --export=_app_loop: This exports the C++ app_loop function so JS can call it.
    #
    cd {{WASM_CRATE_DIR}} && RUSTFLAGS="\
    -Clink-arg=-sENVIRONMENT=web,worker \
    -Clink-arg=-sFILESYSTEM=0 \
    -Clink-arg=-sALLOW_MEMORY_GROWTH=0 \
    -Clink-arg=-o \
    -Clink-arg="{{_RELATIVE_OUTPUT_PATH}}" \
    -Clink-arg=--js-library \
    -Clink-arg="{{_JS_LIBRARY_PATH}}" \
    -Clink-arg=--export=_lv_tick_inc\
    -Clink-arg=--export=_app_loop" \
        cargo build {{BUILD_PROFILE_FLAG}}

    @echo "Copying Emscripten artifacts to {{PKG_DIR}}..."
    @mkdir -p {{PKG_DIR}}

    # Copy the JS file
    @cp {{WASM_FILE_BASE}}.js {{PKG_DIR/WASM_CRATE_NAME}}.js
    # Copy the WASM file (no renaming!)
    @cp {{WASM_FILE_BASE}}.wasm {{PKG_DIR/WASM_CRATE_NAME}}.wasm

    @echo "Build complete. Output in {{PKG_DIR}}"

# Serve the project root
@serve: web
    @echo "Starting web server on http://localhost:8080..."
    # Serve from the WWW_DIR (project root)
    http-server {{WWW_DIR}} -p 8080 --cors --mime .wasm=application/wasm & \
    SERVER_PID=$$!; \
    echo "Server PID: $$SERVER_PID"; \
    trap "echo 'Stopping server...'; kill $$SERVER_PID" EXIT; \
    echo "Watching for changes in {{WASM_CRATE_DIR}}/src, ./src, ./hal, ./gui..."; \
    watchexec -w  ./src -w ./hal -w ./gui  -- \
      "echo 'Rebuilding WASM...' && just web"

@deploy: build-temp upload monitor
