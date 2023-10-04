#pragma once

class Object
{
public:
  Object();
  virtual ~Object();

  virtual void Update(double deltaTime) = 0;
  virtual void OnSpawn() = 0;
  virtual void OnDespawn() = 0;

  const char *GetUUID();

protected:
  char uuid[16];
};