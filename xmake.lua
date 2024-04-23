set_languages("c++17")

add_rules("mode.debug", "mode.check", "mode.release", "mode.releasedbg")
add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})

add_requires("glm", {configs={header_only=false, cxx_standard="17"}})
add_requires("freetype", {configs={bzip2=false, png=false, woff2=false, zlib=false, harfbuzz=false}})
add_requires("imgui", {configs={opengl3=true, sdl2_no_renderer=true}})
add_requires("openrestry-luajit", {configs={gc64=not is_plat("android")}})
add_requires("libsdl", "libsdl_image", "libsdl_mixer")

if not is_plat("android") then
    add_requires("cpptrace", "mimalloc")
end

includes("deps")
includes("engine")
includes("game")

if not is_plat("android") then
    add_requires("toml11")
    includes("tool")
end
