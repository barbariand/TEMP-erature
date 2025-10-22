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
          nativeBuildInputs = [libtiff libwebp sdl3 sdl3-image];
          packages = with pkgs; [gdb cmake platformio arduino-cli arduino-language-server clang-tools];
          shellHook = ''
          '';
        };
    });
}
