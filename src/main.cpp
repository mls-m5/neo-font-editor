#include "dump.h"
#include "fileutils.h"
#include "remap.h"
#include "settings.h"

#ifndef __EMSCRIPTEN__
int main(int argc, char *argv[]) {
    const auto settings = Settings{argc, argv};

    switch (settings.action) {
    case Settings::Dump:
        dumpFont(settings.in, settings.editableFile);
        break;
    case Settings::Encode: {
        auto font = loadDump(settings.editableFile);
        if (!settings.remapFile.empty()) {
            auto remap = Remap{settings.remapFile};
            font = remap.apply(font);
        }
        saveFont(font, settings.outFont);
        std::cout << "font saved to " << settings.outFont << "...\n";

        break;
    }
    case Settings::Repack: {
        auto font = loadFont(settings.in);
        saveFont(font, settings.outFont);
    } break;
    }

    if (settings.shouldVerify && !settings.outFont.empty()) {
        dumpFont(settings.outFont, "");
    }

    return 0;
}

#endif
