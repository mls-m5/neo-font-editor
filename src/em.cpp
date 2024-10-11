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

void createDownload(const NeoFont &font) {
    emscripten::val document = emscripten::val::global("document");

    auto data = std::vector<uint8_t>{};
    data.resize(font.appletSize());
    font.encodeApplet(data.data(), data.size());

    // Create a Blob from the encoded data and make it downloadable
    emscripten::val uint8Array =
        emscripten::val::global("Uint8Array")
            .new_(emscripten::typed_memory_view(data.size(), data.data()));
    emscripten::val blobConstructorParams = emscripten::val::array();
    blobConstructorParams.call<void>("push", uint8Array);
    emscripten::val blob =
        emscripten::val::global("Blob").new_(blobConstructorParams);

    // Create an object URL for the Blob
    emscripten::val url = emscripten::val::global("URL").call<emscripten::val>(
        "createObjectURL", blob);

    // Create an anchor element to trigger the download
    emscripten::val anchor =
        document.call<emscripten::val>("createElement", std::string("a"));
    anchor.set("href", url);

    auto fontName = std::string{font.fontName()};
    for (auto &c : fontName) {
        if (c == ' ') {
            c = '-';
        }
    }
    fontName += ".OS3KApp";
    anchor.set("download", fontName);

    emscripten::val resultDiv = document.call<emscripten::val>(
        "getElementById", std::string{"resultDiv"});
    // Append the anchor to the body and trigger a click to download
    // auto body = document.call<emscripten::val>("body");
    resultDiv.call<void>("appendChild", anchor);
    anchor.call<void>("click");

    // Clean up by revoking the object URL
    emscripten::val::global("URL").call<void>("revokeObjectURL", url);
    resultDiv.call<void>("removeChild", anchor);
}

extern "C" void nativeEncode() {
    emscripten::val document = emscripten::val::global("document");

    auto remap = Remap{};
    auto font = std::make_unique<NeoFont>();
    {
        auto textareaContent = std::string{};
        emscripten::val textArea = document.call<emscripten::val>(
            "getElementById", std::string{"text"});

        textareaContent = textArea["value"].as<std::string>();
        auto ss = std::istringstream{std::move(textareaContent)};
        loadDump(*font, ss);
    }

    {
        auto remapText = std::string{};
        emscripten::val textArea = document.call<emscripten::val>(
            "getElementById", std::string{"translation"});
        remapText = textArea["value"].as<std::string>();
        auto ss = std::istringstream{std::move(remapText)};
        remap.parse(ss);
    }

    auto newFont = remap.apply(*font);

    createDownload(*newFont);
}

#endif
