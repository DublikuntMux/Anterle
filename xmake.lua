set_languages("c++17")
set_arch("x64")

add_rules("mode.debug", "mode.release", "mode.releasedbg")
add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})

add_requires("glm", {configs={header_only=false, cxx_standard="17"}})
add_requires("libsdl", {configs={sdlmain=false}})
add_requires("freetype", {configs={bzip2=false, png=false, woff2=false, zlib=false, harfbuzz=false}})
add_requires("imgui", {configs={opengl3=true, sdl2_no_renderer=true}})
add_requires("fpng", {configs={sse4=true}})
add_requires("openrestry-luajit", {configs={gc64=true}})
add_requires("miniaudio", "stb", "toml11", "cpptrace", "mimalloc")

includes("deps")
includes("engine")
includes("game")
includes("tool")
