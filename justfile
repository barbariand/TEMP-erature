# Justfile provided by Ludvig copyright 2025
set shell := ["bash", "-uc"]
set windows-shell := ["powershell.exe", "-NoLogo", "-Command"]

[positional-arguments, private, unix]
@write *args="":
  echo "$@"

[positional-arguments, private, windows]
@write *args="":
  Write-Host "$@"

[private]
clean_pio:
  @just write Cleaning build files
  pio run --target clean

[private, no-exit-message]
clean_libdeps:
  @just write Cleaning dependencies recursively
  rm -r .pio/libdeps
# list all resepiec
default:
  just --list
# Clean the pio dependencies, libraries, and build files
clean: clean_pio clean_libdeps
test_native:
  pio test -e native
# Generate compile_commands.json
compile_commands:
  pio run -t compiledb

# Generate compile_commands.json for the native target
compile_commands_native:
  pio run -t compiledb -e native

# Build native environment
build-native:
    @just write Building native environment
    pio run -e native
    @just write Native build complete. Run 'just run' to execute.

# Run the natively compiled binary
run-native:
  @just write Executing native build
  ./.pio/build/native/program
  @just write Execution finished.

# Build and run the native environment
native: build-native run-native

# Build ESP32 environment
build-esp32:
  @just write Building ESP32 environment
  pio run -e esp32
  @just write ESP32 build complete. Run 'just upload' to upload to ESP32.

# Upload to ESP32 (T-Display-AMOLED)
[linux]
upload:
    #!/usr/bin/env bash
    if [ ! -e /dev/ttyACM0 ]; then
        @just write ERROR: /dev/ttyACM0 \(T-Display-AMOLED\) does not exist.
        @just write Check USB connection or port name.
        exit 1
    fi
    @just write Uploading to /dev/ttyACM0...
    pio run --target upload --upload-port /dev/ttyACM0

# Open the serial monitor for the esp32 environment.
monitor:
    @just write Starting serial monitor...
    pio device monitor -e esp32

# Builds the ESP32 target, uploads it, and starts the serial monitor.
[linux]
deploy: build-esp32 upload monitor
