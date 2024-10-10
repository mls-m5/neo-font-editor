#ifdef __EMSCRIPTEN__
/// Logic for handling interactions with emscripten

#include "dump.h"
#include "remap.h"
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <emscripten/emscripten.h>
#include <emscripten/val.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <neofontlib/NeoFont.h>
#include <sstream>

extern "C" void nativeHandleDroppedData(uint8_t *data, size_t len) {
    auto font = std::make_unique<NeoFont>();

    // std::cout.write(reinterpret_cast<char *>(data), len);

    if (!font->decodeApplet(data, len)) {
        std::cout << "Failed to decode app/font" << std::endl;
        return;
    }
    std::cout << font->appletName() << std::endl;

    {
        auto ss = std::ostringstream{};

        dump(*font, ss);

        emscripten::val document = emscripten::val::global("document");
        emscripten::val textArea = document.call<emscripten::val>(
            "getElementById", std::string{"text"});
        textArea.set("value", ss.str());
    }

    {
        auto file = std::ifstream{"remap-scheme-uk-sv-dvorak.md"};
        auto ss = std::ostringstream{};
        ss << "# This is an example mapping file, if you are not swedish\n";
        ss << "# You may remove this content\n\n";
        ss << file.rdbuf();

        emscripten::val document = emscripten::val::global("document");
        emscripten::val textArea = document.call<emscripten::val>(
            "getElementById", std::string{"translation"});
        textArea.set("value", ss.str());
    }
}

extern "C" void nativeEncode() {
    auto textareaContent = std::string{};
    auto map = std::string{};

    {
        emscripten::val document = emscripten::val::global("document");
        emscripten::val textArea = document.call<emscripten::val>(
            "getElementById", std::string{"text"});

        textareaContent = textArea["value"].as<std::string>();
    }

    auto remap = Remap{};

    {
        auto ss = std::istringstream{std::move(textareaContent)};
        remap.parse(ss);
    }

    auto font = std::make_unique<NeoFont>();
    {
        // Continu here
    }

    // Todo later
}

#endif
