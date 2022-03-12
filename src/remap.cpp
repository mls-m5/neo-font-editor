#include "remap.h"
#include "alpha.h"
#include "neofontlib/NeoFont.h"
#include <fstream>
#include <iostream>

Remap::Remap(std::filesystem::path path) {
    auto file = std::ifstream{path};

    if (!file.is_open()) {
        std::cerr << "could not open remap file " << path << "\n";
        std::terminate();
    }

    std::cout << "remap file: " << path << "\n";

    auto replacementStrings =
        std::vector<std::pair<std::string, std::string>>{};

    for (std::string line; std::getline(file, line);) {
        if (auto f = line.find("#"); f != std::string::npos) {
            line = line.substr(0, f);
        }

        if (line.empty()) {
            continue;
        }
        else if (auto o = split(line, "->")) {
            auto left = trim(o->first);
            auto right = trim(o->second);
            replacementStrings.emplace_back(left, right);
        }
        else if (auto s = split(line, " ")) {
            auto left = trim(s->first);
            auto right = trim(s->second);
            _definitions[left] = std::stoi(right);
        }
    }

    auto isValid = true;

    auto verify = [&isValid, this](uint8_t c) {
        for (auto pair : _definitions) {
            if (pair.second == c) {
                return;
            }
        }
        std::cerr << "could not find definition for " << static_cast<int>(c)
                  << "\n";

        isValid = false;
    };

    auto findDefinition = [this](std::string name) {
        if (auto f = _definitions.find(name); f != _definitions.end()) {
            return f->second;
        }
        std::cerr << "could not find remap definition for " << name << "\n";
        std::terminate();
    };

    for (auto &pair : replacementStrings) {
        auto left = findDefinition(pair.first);
        auto right = findDefinition(pair.second);
        verify(left);
        verify(right);
        _replacements.emplace_back(left, right);
    }

    if (!isValid) {
        std::cerr << "remap file misses some definitions\n";
        std::terminate();
    }
}

NeoFont Remap::apply(const NeoFont &font) {
    auto newFont = font;

    for (auto replacement : _replacements) {
        newFont.character(replacement.second) =
            font.character(replacement.first);
    }

    return newFont;
}
