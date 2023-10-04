#include "uuid_v4.h"

#include "object/object.hpp"

Object::Object()
{
  UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;
  UUIDv4::UUID uuid_genrator = uuidGenerator.getUUID();
  uuid_genrator.bytes(uuid);
}
Object::~Object() {}

const char *Object::GetUUID() { return uuid; }
