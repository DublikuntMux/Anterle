#include "backend/base.hpp"

namespace Anterle {
class DirectXBackend : public BaseBackend
{
public:
  DirectXBackend(int width, int height, const char *window_name);
  ~DirectXBackend();

  void PreRender();
  void PostRender();

private:
};
}// namespace Anterle
