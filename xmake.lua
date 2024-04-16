set_languages("c++17")

add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})

add_requires("glm", {system=false, configs={header_only=false, cxx_standard="17"}})
add_requires("glfw", {system=false, configs={wayland=is_plat("linux")}})
add_requires("freetype", {system=false, configs={bzip2=false, png=false, woff2=false, zlib=false, harfbuzz=false}})
add_requires("imgui", {system=false, configs={opengl3=true, glfw=true}})
add_requires("fpng", {system=false, configs={sse4=true}})
add_requires("sol2", {system=false, configs={includes_lua=true}})
add_requires("miniaudio", {system=false})
add_requires("stb", {system=false})

includes("deps")
includes("engine")
includes("game")
includes("tool")
