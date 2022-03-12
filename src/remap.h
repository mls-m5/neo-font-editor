#pragma once

#include <filesystem>
#include <map>
#include <vector>

class NeoFont;

struct Remap {
    Remap() = default;
    Remap(std::filesystem::path path);

    [[nodiscard]] NeoFont apply(const NeoFont &font);

private:
    std::map<std::string, uint8_t> _definitions;
    std::vector<std::pair<uint8_t, uint8_t>> _replacements;
};
