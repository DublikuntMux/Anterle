#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <fpng.h>

#include "type/image.hpp"

void processImage(std::string &sourcePath, std::string &destPath)
{
  int width = 0, height = 0, channels = 0;
  unsigned char *image = stbi_load(sourcePath.c_str(), &width, &height, &channels, 0);
  if (image == nullptr) {
    std::cerr << "Failed to load image: " << sourcePath << std::endl;
    return;
  }

  if (!fpng::fpng_encode_image_to_file(destPath.c_str(),
        image,
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height),
        static_cast<uint32_t>(channels),
        fpng::FPNG_ENCODE_SLOWER)) {
    std::cerr << "Failed to save image: " << destPath << std::endl;
  }

  stbi_image_free(image);
}
