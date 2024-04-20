#include <filesystem>
#include <iostream>
#include <string>

#include <SDL_image.h>
#include <cpptrace/cpptrace.hpp>

#include "type/image.hpp"
#include "type/lua_bind.hpp"

namespace fs = std::filesystem;

void processResourceDirectory(const fs::path &sourceDir, const fs::path &destDir)
{
  std::vector<std::string> imageExtensions = {
    ".png",
    ".jpg",
    ".jxl",
    ".webp",
    ".tif",
    ".avif",
  };

  for (const auto &entry : fs::recursive_directory_iterator(sourceDir)) {
    if (std::find(imageExtensions.begin(), imageExtensions.end(), entry.path().extension().string())
        != imageExtensions.end()) {
      fs::path relativePath = fs::relative(entry.path(), sourceDir);
      fs::path destFile = destDir / relativePath;

      std::cout << "Processing: " << relativePath << "\n";

      fs::create_directories(destFile.parent_path());
      std::string sourcePath = entry.path().string();
      std::string destPath = destFile.replace_extension(".png").string();

      processImage(sourcePath, destPath);
    }
  }
}

void processBindingDirectory(const fs::path &sourceDir, const fs::path &destDir)
{
  for (const auto &entry : fs::recursive_directory_iterator(sourceDir)) {
    if (entry.path().extension() == ".toml") {
      fs::path relativePath = fs::relative(entry.path(), sourceDir);
      fs::path destFile = destDir / relativePath;

      fs::create_directories(destFile.parent_path());
      std::string destPath = destFile.replace_extension("").string();
      std::string sourcePath = entry.path().string();

      proccessBindingConfig(sourcePath, destPath);
    }
  }
}

#undef main
int main(int argc, char *argv[])
{
  cpptrace::register_terminate_handler();

  if (argc != 4) {
    std::cerr << "Usage: " << argv[0] << "<source_directory> <destination_directory> <mode>\n";
    return 1;
  }

  int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_JXL | IMG_INIT_TIF | IMG_INIT_WEBP | IMG_INIT_AVIF;
  IMG_Init(imgFlags);

  fs::path sourceDir = argv[1];
  fs::path destDir = argv[2];
  std::string mode = argv[3];

  if (!fs::exists(sourceDir) || !fs::is_directory(sourceDir)) {
    std::cerr << "Source directory does not exist or is not a directory.\n";
    return 1;
  }

  if (!fs::exists(destDir)) {
    fs::create_directories(destDir);
  } else if (!fs::is_directory(destDir)) {
    std::cerr << "Destination path exists but is not a directory.\n";
    return 1;
  }

  if (mode == "asset") {
    processResourceDirectory(sourceDir, destDir);
  } else if (mode == "binding") {
    processBindingDirectory(sourceDir, destDir);
  } else {
    std::cerr << "Mode must be: binding or asset.\n";
  }

  IMG_Quit();

  return 0;
}
