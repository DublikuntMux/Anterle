#include <cstdint>
#include <fstream>
#include <ios>

#include <glad/gles2.h>
#include <loguru.hpp>

#include "utility/screen.hpp"

namespace Anterle {
void saveScreenshotToFile(std::string filename, uint16_t windowWidth, uint16_t windowHeight)
{
  uint64_t numberOfPixels = windowWidth * windowHeight * 3ULL;
  unsigned char *pixels = new unsigned char[numberOfPixels];

  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glReadBuffer(GL_FRONT);
  glReadPixels(0, 0, windowWidth, windowHeight, 0x80E0, GL_UNSIGNED_BYTE, pixels);

  std::ofstream outputFile(filename.c_str(), std::ios::binary);
  if (!outputFile) { LOG_F(ERROR, "Failed to create file for screenshot."); }
  short header[] = { 0, 2, 0, 0, 0, 0, static_cast<short>(windowWidth), static_cast<short>(windowHeight), 24 };
  outputFile.write(reinterpret_cast<const char *>(&header), sizeof(header));
  outputFile.write(
    reinterpret_cast<const char *>(pixels), static_cast<std::streamsize>(numberOfPixels * sizeof(*pixels)));
  outputFile.close();

  delete[] pixels;
}
}// namespace Anterle
