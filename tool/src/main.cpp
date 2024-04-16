#include <filesystem>
#include <iostream>

#include <fpng.h>

#include "type/image.hpp"

namespace fs = std::filesystem;

void processDirectory(const fs::path &sourceDir, const fs::path &destDir)
{
  std::vector<std::string> imageExtensions = { ".png", ".jpg", ".png", ".bmp", ".tga", ".gif", ".hdr", ".pnm", ".psd" };

  for (const auto &entry : fs::recursive_directory_iterator(sourceDir)) {
    if (std::find(imageExtensions.begin(), imageExtensions.end(), entry.path().extension().string())
        != imageExtensions.end()) {
      fs::path relativePath = fs::relative(entry.path(), sourceDir);
      fs::path destPath = destDir / relativePath;

      fs::create_directories(destPath.parent_path());
      processImage(entry.path().string().c_str(), destPath.replace_extension(".png").string().c_str());
    }
  }
}

int main(int argc, char *argv[])
{
  fpng::fpng_init();

  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <source_directory> <destination_directory>\n";
    return 1;
  }

  fs::path sourceDir = argv[1];
  fs::path destDir = argv[2];

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

  processDirectory(sourceDir, destDir);

  return 0;
}
