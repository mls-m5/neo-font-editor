#pragma once

#include <filesystem>
#include <iostream>
#include <vector>

inline const auto helpstr = R"_(
        usage:

   Dump decoded font to terminal
font_editor font-file.os3kapp

or
   Create editable file
font_editor --dump font-file.os3kapp --file font.txt

   Create font from edited file
font_editor --encode --file font.txt --out new-font-file.os3kapp

        arguments:
--dump <file>     (or -d)        decode font file into editable format
--file <file>     (or -f)        specify editable file
--out <file>      (or -o)        specify final font path
--encode <file>   (or -e)        specify configuration file (how to edit font)
--help  -h                       print this text


example config file:
43 -> 60
80 -> 94
)_";

struct Settings {
    using path = std::filesystem::path;

    enum Action {
        Dump,
        Encode,
    };

    path in;
    path editableFile;
    path outFont;

    Action action = Dump;

    static void printHelp() {
        std::cerr << helpstr << "\n";
        std::exit(0);
    }

    void requireOutFile() {
        if (outFont.empty()) {
            std::cerr << "no out font specified. Please use --out\n";
        }
    }

    void requireEditableFile() {
        if (editableFile.empty()) {
            std::cerr << "no editable file specified. Please use --file\n";
        }
    }

    Settings(int argc, char **argv) {
        const auto args = std::vector<std::string>(argv, argv + argc);

        if (args.size() <= 1) {
            printHelp();
        }

        for (size_t i = 1; i < args.size(); ++i) {
            auto getNextArg = [&args, &i] {
                ++i;
                if (i >= args.size()) {
                    std::cerr << "more arguments for action\n";
                    std::exit(0);
                }

                return args.at(i);
            };

            auto arg = args.at(i);

            if (arg == "-e" || arg == "--encode") {
                action = Encode;
            }
            else if (arg == "-d" || arg == "--dump") {
                action = Dump;
            }
            else if (arg == "-f" || arg == "--file") {
                editableFile = getNextArg();
            }
            else if (arg == "-o" || arg == "--out") {
                outFont = getNextArg();
            }
            else if (arg == "-h" || arg == "--help") {
                printHelp();
            }
            else {
                in = arg;
            }
        }

        if (action == Encode) {
            requireEditableFile();
            requireOutFile();
        }
    }
};
