#include "backend/directx.hpp"
#include "backend/base.hpp"

namespace Anterle {
DirectXBackend::DirectXBackend(int width, int height, const char *window_name): BaseBackend(width, height, window_name)
{}
DirectXBackend::~DirectXBackend()
{

}
void DirectXBackend::PreRender()
{
    BaseBackend::PreRender();
}

void DirectXBackend::PostRender()
{
    BaseBackend::PostRender();
}
}
