{ pkgs ? import <nixpkgs> { config.allowUnfree = true; } }:

let
  llvm = pkgs.llvmPackages_latest;

  mymake = pkgs.writeShellScriptBin "mk" ''
    if [ -f "$1.c" ]; then
      i="$1.c"
      c=$CC
    else
      i="$1.cpp"
      c=$CXX
    fi
    o=$1
    shift
    $c -ggdb $i -o $o -lm -Wall $@
  '';
in
pkgs.mkShell {
  name = "C";
  buildInputs = [
    pkgs.gnumake
    pkgs.cmake
    pkgs.ninja
    pkgs.bear
    pkgs.just
    llvm.lldb
    pkgs.gdb
    pkgs.clang-tools
    llvm.libstdcxxClang
    pkgs.cppcheck
    llvm.libllvm
    pkgs.valgrind
    mymake
    pkgs.conan
    llvm.libcxx
    pkgs.libGL
    pkgs.libGLU
    pkgs.faac
    pkgs.faad2
    pkgs.freeglut
    pkgs.glew
    pkgs.glfw
    pkgs.glm
    pkgs.SDL2
    pkgs.SDL2_ttf
    pkgs.SDL2_net
    pkgs.SDL2_gfx
    pkgs.SDL2_sound
    pkgs.SDL2_mixer
    pkgs.SDL2_image
    pkgs.assimp
    pkgs.assimp.dev
  ];
}