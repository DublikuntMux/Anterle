#include <cstdint>

#include <glad/gl.h>

#include <fpng.h>

void saveScreenshot(const char *filename, uint32_t WindowWidth, uint32_t WindowHeight)
{
  GLubyte *pixels = new GLubyte[3 * WindowWidth * WindowHeight];

  glReadPixels(0, 0, WindowWidth, WindowHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels);

  GLubyte *flippedPixels = new GLubyte[3 * WindowWidth * WindowHeight];
  for (int y = 0; y < WindowHeight; ++y) {
    for (int x = 0; x < WindowWidth; ++x) {
      for (int c = 0; c < 3; ++c) {
        flippedPixels[3 * (y * WindowWidth + x) + c] = pixels[3 * ((WindowHeight - y - 1) * WindowWidth + x) + c];
      }
    }
  }

  if (!fpng::fpng_encode_image_to_file(
        filename, flippedPixels, WindowWidth, WindowHeight, 3, fpng::FPNG_ENCODE_SLOWER)) {}

  delete[] pixels;
  delete[] flippedPixels;
}