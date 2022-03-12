#pragma once

#include <filesystem>
#include <vector>

inline const auto helpstr = R"_(
usage:

font_editor font-file.os3kapp

or

font_editor font-file.os3kapp --config config-file.txt --out new-font-file.os3kapp

example config file:
43 -> 60
80 -> 94
)_";

struct Settings {
    using path = std::filesystem::path;

    path in;
    path out;
    path config;

    Settings(int argc, char **argv) {
        const auto args = std::vector<std::string>(argv, argv + argc);

        for (size_t i = 1; i < args.size(); ++i) {
            auto arg = args.at(i);

            if (arg == "-o" || arg == "--out") {
                out = args.at(++i);
            }
            else if (arg == "-c" || arg == "--config") {
                config = args.at(++i);
            }
            else {
                in = arg;
            }
        }
    }
};
