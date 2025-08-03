# Justfile for cuddly-tribble

build:
    mkdir -p build
    cmake -S . -B build
    cmake --build build

run: build
    ./build/cuddly_tribble

dev: run

clean:
    rm -rf build

clean_build: clean
    build

nix:
    nix-shell "./shell.nix"