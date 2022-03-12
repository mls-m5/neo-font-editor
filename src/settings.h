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
--remap <file>    (or -r)        remap (swap) letters according to remap file
--verify                         when encoding print to terminal to inspect
--repack                         (for testing) decode applet and encode
--help  -h                       print this text


)_";

struct Settings {
    using path = std::filesystem::path;

    enum Action {
        Dump,
        Encode,
        Repack,
    };

    path in;
    path editableFile;
    path outFont;
    path remapFile;
    bool shouldVerify = false;

    Action action = Dump;

    static void printHelp() {
        std::cerr << helpstr << "\n";
        std::terminate();
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

    void requireInFile() {
        if (in.empty()) {
            std::cerr << "no in font file specified.\n";
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
                    std::terminate();
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
            else if (arg == "-r" || arg == "--remap") {
                remapFile = getNextArg();
            }
            else if (arg == "--verify") {
                shouldVerify = true;
            }
            else if (arg == "--repack") {
                action = Repack;
            }
            else if (arg == "-h" || arg == "--help") {
                printHelp();
            }
            else {
                in = arg;
            }
        }

        switch (action) {
        case Encode:
            requireEditableFile();
            requireOutFile();
            break;
        case Dump:
            requireInFile();
            break;
        case Repack:
            requireInFile();
            requireOutFile();
        }
    }
};
