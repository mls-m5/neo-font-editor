
add_subdirectory(lib/neofontlib)

add_executable(
    font_editor
    src/main.cpp
    src/dump.cpp
    )

target_link_libraries(
    font_editor
    neo_font_lib
    )
