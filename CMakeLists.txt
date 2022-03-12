
add_subdirectory(lib/neofontlib)

add_executable(
    font_editor
    src/main.cpp
    )

target_link_libraries(
    font_editor
    neo_font_lib
    SDL2_image
    SDL2
    )
