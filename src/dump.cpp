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
    d("appletinfo", font.appletInfo());
    d("fontname", font.fontName());
    d("version", font.version());
    d("id", font.ident());
    d("height", font.height());
    stream << "\n";

    for (auto &c : font) {
        unsigned char letter = std::distance(font.begin(), &c);
        d("char", static_cast<int>(letter));
        d("width", c.width());
        d("height", c.height());
        stream << "\n";

        printCharacter(c, stream);
    }
}

NeoCharacter loadChar(std::istream &stream, size_t width, size_t height) {
    auto c = NeoCharacter{};

    c.setWidth(width);
    c.setHeight(height);

    auto y = 0ul;

    auto started = false;

    for (std::string line; std::getline(stream, line);) {
        if (line.empty()) {
            if (started) {
                break;
            }
            else {
                continue;
            }
        }

        started = true;
        if (y >= height) {
            continue; // Dont break, we need to flush remainding lines
        }
        auto x = 0ul;
        for (auto letter : line) {
            if (x >= width) {
                break;
            }
            if (letter == 'x' || letter == 'X') {
                c.setPixel(x, y);
            }
            else {
                c.clearPixel(x, y);
            }
            ++x;
        }
        ++y;
    }

    return c;
}

} // namespace

void dumpFont(std::filesystem::path path, std::filesystem::path target) {
    auto font = loadFont(path);

    if (target.empty()) {
        dump(font);
    }
    else {
        auto file = std::ofstream{target};
        dump(font, file);
    }
}

NeoFont loadDump(std::filesystem::path path) {
    auto file = std::ifstream{path};

    if (!file.is_open()) {
        std::cerr << "could not open path " << path << "\n";
        std::terminate();
    }

    auto getLine = [&file]() {
        std::string line;
        std::getline(file, line);
        return line;
    };

    auto getNonZeroLine = [&]() {
        auto line = getLine();
        while (line.empty()) {
            if (!file) {
                return line;
            }
            line = getLine();
        }
        return line;
    };

    auto loadProperty = [&getNonZeroLine](std::string name) {
        auto line = getNonZeroLine();
        if (line.substr(0, name.size() + 1) != (name + " ")) {
            std::cerr << "expected property '" << name << "', got '" << line
                      << "'\n";
            std::terminate();
        }
        return line.substr(name.size() + 1);
    };

    auto font = NeoFont{};

    font.setAppletName(loadProperty("appletname").c_str());
    font.setAppletInfo(loadProperty("appletinfo").c_str());
    font.setFontName(loadProperty("fontname").c_str());
    font.setVersion(loadProperty("version").c_str());
    font.setIdent(std::stoi(loadProperty("id")));
    font.setHeight(std::stoi(loadProperty("height")));

    auto loaded = std::array<bool, 256>{};

    for (std::string line; line = getNonZeroLine(), !line.empty();) {
        if (!line.starts_with("char ")) {
            std::cerr << "expected 'char' got " << line << "\n";
            std::terminate();
        }
        auto index = std::stoul(line.substr(5));

        auto width = std::stoul(loadProperty("width"));
        auto height = std::stoul(loadProperty("height"));

        font.character(index) = loadChar(file, width, height);

        loaded.at(index) = true;
    }

    std::cout << "font loaded..." << std::endl;

    for (size_t i = 0; i < loaded.size(); ++i) {
        if (!loaded.at(i)) {
            std::cerr << "warning character " << i << " was never loaded\n";
        }
    }

    return font;
}
