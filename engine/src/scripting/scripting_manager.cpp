#include <sol/sol.hpp>

#include "scripting/glm/vector.hpp"

#include "scripting/script_manager.hpp"

namespace Anterle {
ScriptManager::ScriptManager() : Lua(sol::state())
{
  Lua.open_libraries(sol::lib::base,
    sol::lib::string,
    sol::lib::math,
    sol::lib::io,
    sol::lib::table,
    sol::lib::package,
    sol::lib::utf8,
    sol::lib::coroutine);

  Scripting::BindGLMVectors(Lua);
}

void ScriptManager::loadScript(const char *script) { Lua.safe_script(script); }
}// namespace Anterle
