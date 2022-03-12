#pragma once

#include "neofontlib/NeoFont.h"
#include <filesystem>

// Dumps a font from a path to target path
// if @target is empty, result is dumped to stdout
void dumpFont(std::filesystem::path path, std::filesystem::path target);

NeoFont loadDump(std::filesystem::path path);
