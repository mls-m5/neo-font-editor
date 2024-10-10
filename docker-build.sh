#!/bin/sh

echo building with emscripten docker image

mkdir -p build/emdocker

rm build/emdocker/index.html

docker run --rm \
    -v $(pwd):$(pwd) \
    -u $(id -u):$(id -g) \
    -w $(pwd) emscripten/emsdk \
    emcc -gsource-map src/*.cpp lib/neofontlib/src/*.cc -I include -I lib/neofontlib/include/ \
        -std=c++20 -o build/emdocker/index.html --shell-file html/index.html \
        -sEXPORTED_FUNCTIONS='[_nativeHandleDroppedData, "_malloc", "_free"]' \
        --cache build/emcache \
        --bind \
        --embed-file remap-scheme-uk-sv-dvorak.md


