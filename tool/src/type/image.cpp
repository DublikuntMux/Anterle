#include <iostream>

#include <SDL_image.h>

#include "type/image.hpp"

void processImage(std::string &sourcePath, std::string &destPath)
{
  SDL_Surface *loadedSurface = IMG_Load(sourcePath.c_str());
  if (loadedSurface == nullptr) {
    std::cerr << "Unable to load image " << sourcePath << "! SDL_image Error: " << IMG_GetError() << std::endl;
    return;
  }

  if (IMG_SavePNG(loadedSurface, destPath.c_str()) != 0) {
    std::cerr << "Unable to save image as PNG! SDL_image Error: " << IMG_GetError() << std::endl;
  }

  SDL_FreeSurface(loadedSurface);
}
