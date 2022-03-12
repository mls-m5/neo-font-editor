#pragma once

#include <filesystem>
#include <fstream>
#include <vector>

inline std::vector<char> loadFile(std::filesystem::path path) {
    auto file = std::ifstream{path};

    return std::vector<char>{std::istreambuf_iterator<char>{file}, {}};
}

template <typename T>
void saveFile(std::filesystem::path path, const T &data) {
    std::ofstream{path, std::ios::binary}.write(data.data(), data.size());
}
