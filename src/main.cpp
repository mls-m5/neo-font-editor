#include "dump.h"
#include "fileutils.h"
#include "settings.h"

int main(int argc, char *argv[]) {
    const auto settings = Settings{argc, argv};

    switch (settings.action) {
    case Settings::Dump:
        dumpFont(settings.in, settings.editableFile);
        break;
    case Settings::Encode:
        auto font = loadDump(settings.editableFile);
        saveFont(font, settings.outFont);
        std::cout << "font saved...";

        break;
    }

    return 0;
}
