#include "dump.h"
#include "fileutils.h"
#include <iostream>

namespace {

void printCharacter(const NeoCharacter &c, std::ostream &stream) {
    for (size_t y = 0; y < c.height(); ++y) {
        for (size_t x = 0; x < c.width(); ++x) {
            stream << (c.getPixel(x, y) ? 'x' : ' ');
        }

        stream << ".\n";
    }
    stream << "\n";
}

void dump(const NeoFont &font, std::ostream &stream = std::cout) {
    // Print named property
    auto d = [&stream](auto name, auto value) {
        stream << name << " " << value << "\n";
    };

    d("appletname", font.appletName());
    d("appletInfo", font.appletInfo());
    d("fontName", font.fontName());
    d("version", font.version());
    d("id", font.ident());
    d("height", font.height());

    for (auto &c : font) {
        unsigned char letter = std::distance(font.begin(), &c);
        d("char", static_cast<int>(letter));
        d("width", c.width());
        d("height", c.height());
        stream << "\n";

        printCharacter(c, stream);
    }
}

} // namespace

void dumpFont(std::filesystem::path path, std::filesystem::path target) {
    auto content = loadFile(path);

    auto font = NeoFont{};
    font.decodeApplet(content);

    if (target.empty()) {
        dump(font);
    }
    else {
        auto file = std::ofstream{target};
        dump(font, file);
    }
}
