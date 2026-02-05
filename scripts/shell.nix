{ pkgs ? import <nixpkgs> {} }:
  pkgs.mkShell {
    # nativeBuildInputs is usually what you want -- tools you need to run
    nativeBuildInputs = with pkgs.buildPackages; [ 
      emscripten 
      emscriptenStdenv 
      live-server
      cmake
      pkg-config
      glfw
      docker
      nodejs_24
    ];
}
