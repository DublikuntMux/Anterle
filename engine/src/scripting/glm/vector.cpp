#include <glm/glm.hpp>

#include "scripting/glm/generated/vectors/vec2.hpp"
#include "scripting/glm/generated/vectors/vec3.hpp"
#include "scripting/glm/generated/vectors/vec4.hpp"
#include "scripting/glm/vector.hpp"
#include "sol/sol.hpp"


namespace Anterle::Scripting {
void BindVectorOperation(sol::state &lua)
{
  lua.set_function("distance",
    sol::overload([](glm::vec2 &a, glm::vec2 &b) { return glm::distance(a, b); },
      [](glm::vec3 &a, glm::vec3 &b) { return glm::distance(a, b); },
      [](glm::vec4 &a, glm::vec4 &b) { return glm::distance(a, b); }));

  lua.set_function("clamp",
    sol::overload([](float value, float min, float max) { return std::clamp(value, min, max); },
      [](double value, double min, double max) { return std::clamp(value, min, max); },
      [](int value, int min, int max) { return std::clamp(value, min, max); }));

  lua.set_function("dot",
    sol::overload([](glm::vec2 &a, glm::vec2 &b) { return glm::dot(a, b); },
      [](glm::vec3 &a, glm::vec3 &b) { return glm::dot(a, b); },
      [](glm::vec4 &a, glm::vec4 &b) { return glm::dot(a, b); }));
}

void BindGLMVectors(sol::state &lua)
{
  bind_vec2(lua);
  bind_vec3(lua);
  bind_vec4(lua);
  BindVectorOperation(lua);
}
}// namespace Anterle::Scripting
