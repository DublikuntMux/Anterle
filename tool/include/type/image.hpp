#pragma once

#include <filesystem>

namespace fs = std::filesystem;

void processImage(const fs::path &sourcePath, const fs::path &destPath);
