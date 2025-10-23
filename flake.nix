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
    in {
      devShells.default = with pkgs;
        mkShell {
          nativeBuildInputs = [arduino-core libtiff libwebp SDL2 SDL_image];
          packages = with pkgs; [gdb cmake platformio arduino-cli arduino-language-server clang-tools];
          shellHook = ''
            export Arduino_DIR="${arduino-core}"
          '';
        };
    });
}
