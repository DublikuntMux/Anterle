// This file generated by AnterleTool.
// Dont modifiy it.
// clang-format off

#include "scripting/glm/generated/vectors/vec4.hpp"

namespace Anterle::Scripting {
void bind_vec4(sol::state &lua)
{
auto multiplication_overloads = sol::overload(
    [](const glm::vec4 &v1, glm::vec4 v2) { return v1 * v2; },
    [](const glm::vec4 &v1, float v2) { return v1 * v2; },
    [](const float &v1, glm::vec4 v2) { return v1 * v2; }
);
auto division_overloads = sol::overload(
    [](const glm::vec4 &v1, glm::vec4 v2) { return v1 / v2; },
    [](const glm::vec4 &v1, float v2) { return v1 / v2; },
    [](const float &v1, glm::vec4 v2) { return v1 / v2; }
);
auto addition_overloads = sol::overload(
    [](const glm::vec4 &v1, glm::vec4 v2) { return v1 + v2; },
    [](const glm::vec4 &v1, float v2) { return v1 + v2; },
    [](const float &v1, glm::vec4 v2) { return v1 + v2; }
);
auto subtraction_overloads = sol::overload(
    [](const glm::vec4 &v1, glm::vec4 v2) { return v1 - v2; },
    [](const glm::vec4 &v1, float v2) { return v1 - v2; },
    [](const float &v1, glm::vec4 v2) { return v1 - v2; }
);
    lua.new_usertype<glm::vec4>("vec4",
        sol::call_constructor,
        sol::constructors<glm::vec4(float), glm::vec4(float, float, float, float)>(),
        "x", &glm::vec4::x,
        "y", &glm::vec4::y,
        "z", &glm::vec4::z,
        "w", &glm::vec4::w,
        "r", &glm::vec4::r,
        "g", &glm::vec4::g,
        "b", &glm::vec4::b,
        "a", &glm::vec4::a,
        "s", &glm::vec4::s,
        "t", &glm::vec4::t,
        "p", &glm::vec4::p,
        "q", &glm::vec4::q,
        sol::meta_function::multiplication, multiplication_overloads,
        sol::meta_function::division, division_overloads,
        sol::meta_function::addition, addition_overloads,
        sol::meta_function::subtraction, subtraction_overloads,
        "length", [](const glm::vec4 &v) { return glm::length(v); }
    );
}
}// namespace Anterle::Scripting
// clang-format on
