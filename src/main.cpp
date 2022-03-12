#include "dump.h"
#include "settings.h"

int main(int argc, char *argv[]) {
    const auto settings = Settings{argc, argv};

    switch (settings.action) {
    case Settings::Dump:
        dumpFont(settings.in, settings.editableFile);
        break;
    case Settings::Encode:
        auto font = loadDump(settings.editableFile);
        break;
    }

    return 0;
}
