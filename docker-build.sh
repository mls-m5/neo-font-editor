#!/bin/sh

echo building with emscripten docker image

mkdir -p build/emdocker

docker run --rm \
    -v $(pwd):$(pwd) \
    -u $(id -u):$(id -g) \
    -w $(pwd) emscripten/emsdk \
    emcc src/*.cpp lib/neofontlib/src/*.cc -I include -I lib/neofontlib/include/ \
        -std=c++20 -o build/emdocker/lib.js


