{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs";
    nixpkgs_unstable.url = "github:nixos/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs = {
    flake-utils,
    nixpkgs,
    nixpkgs_unstable,
    ...
  }:
    flake-utils.lib.eachDefaultSystem (system: let
      pkgs = import nixpkgs {inherit system;} // {unstable = import nixpkgs_unstable {inherit system;};};
    in {
      devShells.default = with pkgs;
        mkShell {
          nativeBuildInputs = [
            libtiff
            libwebp
            SDL2
            SDL_image
          ];
          packages = with pkgs; [
            python3
            gdb
            cmake
            platformio
            clang-tools
            http-server
            watchexec
            unstable.just
          ];
          shellHook = ''
            export Arduino_DIR="${arduino-core}"
            export SDL2_INCLUDE_PATH="${SDL2.dev}/include"
            export SDL2_LIBRARY_PATH="${SDL2}/lib"
            export CXXFLAGS="$CXXFLAGS -I$SDL2_INCLUDE_PATH -L$SDL2_LIBRARY_PATH"
            export LIBCLANG_PATH="${libclang.lib}/lib"
            export EMSCRIPTEN_SYSTEM_PATH="${pkgs.emscripten}/share/emscripten/system"
          '';
        };
    });
}
