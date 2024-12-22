set_languages("c++20")

add_rules("mode.debug", "mode.check", "mode.releasedbg")
add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})

add_requires("glm", {configs={header_only=false, cxx_standard="17"}})
add_requires("freetype", {configs={bzip2=false, png=false, woff2=false, zlib=false, harfbuzz=false}})
add_requires("imgui", {configs={opengl3=true, sdl2_no_renderer=true}})
add_requires("openrestry-luajit", {configs={gc64=true}})
add_requires("libsdl", "libsdl_image", "libsdl_mixer")
add_requires("cpptrace", "mimalloc", "toml11")

includes("deps")
includes("engine")
includes("game")
includes("tool")
