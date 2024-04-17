#include <glm/glm.hpp>

#include "scripting/glm/vector.hpp"

namespace Andterle::Scripting {
void BindVec2(sol::state &lua)
{
  auto vec2_multiply_overloads = sol::overload([](const glm::vec2 &v1, const glm::vec2 &v2) { return v1 * v2; },
    [](const glm::vec2 &v1, float v2) { return v1 * v2; },
    [](float v1, const glm::vec2 &v2) { return v1 * v2; });

  auto vec2_divide_overloads = sol::overload([](const glm::vec2 &v1, const glm::vec2 &v2) { return v1 / v2; },
    [](const glm::vec2 &v1, float v2) { return v1 / v2; },
    [](float v1, const glm::vec2 &v2) { return v1 / v2; });

  auto vec2_addition_overloads = sol::overload([](const glm::vec2 &v1, const glm::vec2 &v2) { return v1 + v2; },
    [](const glm::vec2 &v1, float v2) { return v1 + v2; },
    [](float v1, const glm::vec2 &v2) { return v1 + v2; });

  auto vec2_subtraction_overloads = sol::overload([](const glm::vec2 &v1, const glm::vec2 &v2) { return v1 - v2; },
    [](const glm::vec2 &v1, float v2) { return v1 - v2; },
    [](float v1, const glm::vec2 &v2) { return v1 - v2; });

  lua.new_usertype<glm::vec2>("vec2",
    sol::call_constructor,
    sol::constructors<glm::vec2(float), glm::vec2(float, float)>(),
    "x",
    &glm::vec2::x,
    "y",
    &glm::vec2::y,
    sol::meta_function::multiplication,
    vec2_multiply_overloads,
    sol::meta_function::division,
    vec2_divide_overloads,
    sol::meta_function::addition,
    vec2_addition_overloads,
    sol::meta_function::subtraction,
    vec2_subtraction_overloads,
    "length",
    [](const glm::vec2 &v) { return glm::length(v); });
}

void BindVec3(sol::state &lua)
{
  auto vec3_multiply_overloads = sol::overload([](const glm::vec3 &v1, const glm::vec3 &v2) { return v1 * v2; },
    [](const glm::vec3 &v1, float v2) { return v1 * v2; },
    [](float v1, const glm::vec3 &v2) { return v1 * v2; });

  auto vec3_divide_overloads = sol::overload([](const glm::vec3 &v1, const glm::vec3 &v2) { return v1 / v2; },
    [](const glm::vec3 &v1, float v2) { return v1 / v2; },
    [](float v1, const glm::vec3 &v2) { return v1 / v2; });

  auto vec3_addition_overloads = sol::overload([](const glm::vec3 &v1, const glm::vec3 &v2) { return v1 + v2; },
    [](const glm::vec3 &v1, float v2) { return v1 + v2; },
    [](float v1, const glm::vec3 &v2) { return v1 + v2; });

  auto vec3_subtraction_overloads = sol::overload([](const glm::vec3 &v1, const glm::vec3 &v2) { return v1 - v2; },
    [](const glm::vec3 &v1, float v2) { return v1 - v2; },
    [](float v1, const glm::vec3 &v2) { return v1 - v2; });

  lua.new_usertype<glm::vec3>("vec3",
    sol::call_constructor,
    sol::constructors<glm::vec3(float), glm::vec3(float, float, float)>(),
    "x",
    &glm::vec3::x,
    "y",
    &glm::vec3::y,
    "z",
    &glm::vec3::z,
    sol::meta_function::multiplication,
    vec3_multiply_overloads,
    sol::meta_function::division,
    vec3_divide_overloads,
    sol::meta_function::addition,
    vec3_addition_overloads,
    sol::meta_function::subtraction,
    vec3_subtraction_overloads,
    "length",
    [](const glm::vec3 &v) { return glm::length(v); });
}

void BindVec4(sol::state &lua)
{
  auto vec4_multiply_overloads = sol::overload([](const glm::vec4 &v1, const glm::vec4 &v2) { return v1 * v2; },
    [](const glm::vec4 &v1, float v2) { return v1 * v2; },
    [](float v1, const glm::vec4 &v2) { return v1 * v2; });

  auto vec4_divide_overloads = sol::overload([](const glm::vec4 &v1, const glm::vec4 &v2) { return v1 / v2; },
    [](const glm::vec4 &v1, float v2) { return v1 / v2; },
    [](float v1, const glm::vec4 &v2) { return v1 / v2; });

  auto vec4_addition_overloads = sol::overload([](const glm::vec4 &v1, const glm::vec4 &v2) { return v1 + v2; },
    [](const glm::vec4 &v1, float v2) { return v1 + v2; },
    [](float v1, const glm::vec4 &v2) { return v1 + v2; });

  auto vec4_subtraction_overloads = sol::overload([](const glm::vec4 &v1, const glm::vec4 &v2) { return v1 - v2; },
    [](const glm::vec4 &v1, float v2) { return v1 - v2; },
    [](float v1, const glm::vec4 &v2) { return v1 - v2; });

  lua.new_usertype<glm::vec4>("vec4",
    sol::call_constructor,
    sol::constructors<glm::vec4(float), glm::vec4(float, float, float, float)>(),
    "x",
    &glm::vec4::x,
    "y",
    &glm::vec4::y,
    "z",
    &glm::vec4::z,
    "w",
    &glm::vec4::w,
    sol::meta_function::multiplication,
    vec4_multiply_overloads,
    sol::meta_function::division,
    vec4_divide_overloads,
    sol::meta_function::addition,
    vec4_addition_overloads,
    sol::meta_function::subtraction,
    vec4_subtraction_overloads,
    "length",
    [](const glm::vec4 &v) { return glm::length(v); });
}

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
}

void BindGLMVectors(sol::state &lua)
{
  BindVec2(lua);
  BindVec3(lua);
  BindVec4(lua);

  BindVectorOperation(lua);
}
}// namespace Andterle::Scripting
