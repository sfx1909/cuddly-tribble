# Justfile for cuddly-tribble

set shell := ["fish", "-c"]

build:
    mkdir -p build
    cd build; cmake ..
    cd build; make

run: build
    cd build; ./cuddly_tribble

dev: run
