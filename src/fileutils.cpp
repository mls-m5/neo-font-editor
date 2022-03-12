#include "fileutils.h"
#include "neofontlib/NeoFont.h"

std::vector<char> loadFile(std::filesystem::path path) {
    auto file = std::ifstream{path};

    return std::vector<char>{std::istreambuf_iterator<char>{file}, {}};
}

void saveFont(const NeoFont &font, std::filesystem::path path) {
    auto data = font.encodeApplet();
    std::ofstream{path}.write(data.data(), data.size());
}

NeoFont loadFont(std::filesystem::path path) {
    auto content = loadFile(path);

    auto font = NeoFont{};
    font.decodeApplet(content);
    return font;
}
