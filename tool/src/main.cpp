#include <filesystem>
#include <fstream>
#include <iostream>

#include <fpng.h>
#include <glslang/Public/ShaderLang.h>

#include "type/image.hpp"
#include "type/shader.hpp"

namespace fs = std::filesystem;

void processDirectory(const fs::path &sourceDir, const fs::path &destDir)
{
  for (const auto &entry : fs::recursive_directory_iterator(sourceDir)) {
    if (entry.path().extension() == ".png") {
      fs::path relativePath = fs::relative(entry.path(), sourceDir);
      fs::path destPath = destDir / relativePath;

      fs::create_directories(destPath.parent_path());
      processImage(entry.path(), destPath);

    } else if (entry.path().extension() == ".frag") {
      fs::path relativePath = fs::relative(entry.path(), sourceDir);
      fs::path destPath = destDir / relativePath;
      destPath += ".spv";

      std::ifstream file(entry.path());
      fs::create_directories(destPath.parent_path());
      compileGLSLToSPIRV(std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()),
        destPath.string(),
        EShLangFragment);
      file.close();

    } else if (entry.path().extension() == ".vert") {
      fs::path relativePath = fs::relative(entry.path(), sourceDir);
      fs::path destPath = destDir / relativePath;
      destPath += ".spv";

      std::ifstream file(entry.path());
      fs::create_directories(destPath.parent_path());
      compileGLSLToSPIRV(std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()),
        destPath.string(),
        EShLangVertex);
      file.close();
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
