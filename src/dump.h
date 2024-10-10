#pragma once

#include "neofontlib/NeoFont.h"
#include <filesystem>
#include <ostream>

// Dumps a font from a path to target path
// if @target is empty, result is dumped to stdout
void dump(const NeoFont &font, std::ostream &stream);
void dumpFont(std::filesystem::path path, std::filesystem::path target);

void loadDump(NeoFont &font, std::filesystem::path path);
void loadDump(NeoFont &font, std::istream &file);
