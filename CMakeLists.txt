
cmake_minimum_required(VERSION 3.22)

project(neo_font_editor)

add_subdirectory(lib/neofontlib)

add_executable(
    font_editor
    src/main.cpp
    src/dump.cpp
    src/fileutils.cpp
    src/remap.cpp
    )

target_link_libraries(
    font_editor
    neo_font_lib
    )

target_compile_features(
    neo_font_lib
    PUBLIC
    cxx_std_20)

file(
    COPY
    remap-scheme-uk-sv-dvorak.md
    DESTINATION
    .)
