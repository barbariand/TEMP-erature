
## Print the current build environment details (Nix variables)
@env-info:
    @echo "--- Environment Check ---"
    @echo "SDL Include Path: $(SDL2_INCLUDE_PATH)"
    @echo "SDL Library Path: $(SDL2_LIBRARY_PATH)"
    @echo "Arduino Dir: $(Arduino_DIR)"
    @echo "PlatformIO Version:"
    @pio --version
    @echo "--- Nix Shell Active ---"

## Remove all built files, including dependencies and libraries.
@clean:
    @echo "Cleaning all build artifacts for all environments..."
    pio run --target clean
    @echo "Cleaning library dependencies..."
    rm -rf .pio/libdeps

@clangd:
  @pio run -t compiledb

@clangd-native:
  @pio run -t compiledb -e native
#-----------------------------------------------------------------------------
# NATIVE BUILD & RUN (SDL)
#-----------------------------------------------------------------------------

## Build the native executable (desktop simulation).
@native:
    @echo "Building native environment (SDL)..."
    pio run -e native
    @echo "Native build complete. Run 'just run' to execute."

## Run the compiled native executable (only works after 'just native').
# Assumes the executable is named 'program' in the native build folder.
@run:
    @just native || exit 1 # Ensure build succeeds before running
    @echo "Executing native program..."
    ./.pio/build/native/program
    @echo "Execution finished."

#-----------------------------------------------------------------------------
# EMBEDDED BUILD & UPLOAD (ESP32 - Temp)
#-----------------------------------------------------------------------------

## Build the firmware for the ESP32 (Temp environment).
@build-temp:
    @echo "Building ESP32 environment (Temp)..."
    pio run -e Temp

## Upload the compiled firmware to the T-Display-AMOLED board.
# Checks for the expected port before attempting upload.
@upload:
    @if [ ! -e /dev/ttyACM0 ]; then \
        echo "ERROR: /dev/ttyACM0 (T-Display-AMOLED) does not exist."; \
        echo "Check USB connection or port name."; \
        exit 1; \
    fi
    @echo "Uploading to /dev/ttyACM0..."
    pio run --target upload --upload-port /dev/ttyACM0

## Open the serial monitor for the Temp environment.
@monitor:
    @echo "Starting serial monitor..."
    pio device monitor -e Temp

#-----------------------------------------------------------------------------
# COMBINED/UTILITY
#-----------------------------------------------------------------------------

## Builds the ESP32 target, uploads it, and starts the serial monitor.
@deploy:
    @just build-temp
    @just upload
    @just monitor
