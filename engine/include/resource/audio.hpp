#pragma once

namespace Anterle {
class Audio
{
public:
  Audio() = default;
  void Asign(void *data);

public:
  void *chunk = nullptr;
};

}// namespace Anterle
