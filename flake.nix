{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs = {
    flake-utils,
    nixpkgs,
    ...
  }:
    flake-utils.lib.eachDefaultSystem (system: let
      pkgs = import nixpkgs {inherit system;};

      # Get the Emscripten package
      emscripten-pkg = pkgs.emscripten;
    in {
      devShells.default = with pkgs;
        mkShell {
          nativeBuildInputs = [
            arduino-core
            libtiff
            libwebp
            SDL2
            SDL_image
          ];
          packages = with pkgs; [
            gdb
            cmake
            platformio
            arduino-cli
            arduino-language-server
            clang-tools
            emscripten-pkg
            libclang # For LIBCLANG_PATH
            wasm-bindgen-cli_0_2_100
            wasm-pack
            http-server
            watchexec # <--- Add watchexec
          ];
          shellHook = ''
            export Arduino_DIR="${arduino-core}"
            export SDL2_INCLUDE_PATH="${SDL2.dev}/include"
            export SDL2_LIBRARY_PATH="${SDL2}/lib"
            export CXXFLAGS="$CXXFLAGS -I$SDL2_INCLUDE_PATH -L$SDL2_LIBRARY_PATH"

            # 1. For clang-sys (finding libclang.so)
            export LIBCLANG_PATH="${libclang.lib}/lib"

            # 2. For build.rs (finding headers)
            export EMSCRIPTEN_SYSTEM_PATH="${emscripten-pkg}/share/emscripten/system"
          '';
        };
    });
}
