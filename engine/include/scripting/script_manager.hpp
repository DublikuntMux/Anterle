#pragma once


#include <sol/sol.hpp>

namespace Anterle {
class ScriptManager
{
public:
    ScriptManager();

    void loadScript(const char * script);

    void resetSnapshot();
    void makeSnapshot();


private:
    sol::state Lua;
};
}
