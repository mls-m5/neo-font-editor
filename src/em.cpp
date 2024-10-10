#ifdef __EMSCRIPTEN__
/// Logic for handling interactions with emscripten

#include "dump.h"
#include "remap.h"
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <emscripten/emscripten.h>
#include <emscripten/val.h>
#include <iostream>
#include <memory>
#include <neofontlib/NeoFont.h>
#include <sstream>

extern "C" void nativeHandleDroppedData(uint8_t *data, size_t len) {
    auto font = std::make_unique<NeoFont>();
    font->decodeApplet(data, len);
    std::cout << font->appletName() << std::endl;

    {
        auto ss = std::ostringstream{};

        dump(*font, ss);

        emscripten::val document = emscripten::val::global("document");
        emscripten::val textArea = document.call<emscripten::val>(
            "getElementById", std::string{"text"});
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

    // Todo later
}

#endif
