#include <fstream>
#include <vector>

#include <glad/gles2.h>
#include <loguru.hpp>

#include "utility/screen.hpp"

namespace Anterle {
void saveScreenshotToFile(std::string filename, int windowWidth, int windowHeight)
{
  const int numberOfPixels = windowWidth * windowHeight * 3;
  unsigned char *pixels = new unsigned char[numberOfPixels];

  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glReadBuffer(GL_FRONT);
  glReadPixels(0, 0, windowWidth, windowHeight, 0x80E0, GL_UNSIGNED_BYTE, pixels);

  std::ofstream outputFile(filename.c_str(), std::ios::binary);
  if (!outputFile) { LOG_F(ERROR, "Failed to create file for screenshot."); }
  short header[] = { 0, 2, 0, 0, 0, 0, static_cast<short>(windowWidth), static_cast<short>(windowHeight), 24 };
  outputFile.write(reinterpret_cast<const char *>(&header), sizeof(header));
  outputFile.write(reinterpret_cast<const char *>(pixels), numberOfPixels * sizeof(*pixels));
  outputFile.close();

  delete[] pixels;
}
}// namespace Anterle
