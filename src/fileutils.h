#pragma once

#include <filesystem>
#include <fstream>
#include <vector>

inline std::vector<char> loadFile(std::filesystem::path path);

template <typename T>
void saveFile(std::filesystem::path path, const T &data);

class NeoFont loadFont(std::filesystem::path path);
void saveFont(const NeoFont &font, std::filesystem::path path);

// Declarations

template <typename T>
void saveFile(std::filesystem::path path, const T &data) {
    std::ofstream{path, std::ios::binary}.write(data.data(), data.size());
}
