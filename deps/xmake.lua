target("glad")
    set_kind("static")
    add_files("glad/src/**.c")
    add_includedirs("glad/include", {public = true})

target("sol")
    set_kind("headeronly")
    add_headerfiles("sol2/**.hpp")
    add_includedirs("sol2/", {public = true})
    add_rules("utils.install.cmake_importfiles")
    add_rules("utils.install.pkgconfig_importfiles")
    add_defines("SOL_ALL_SAFETIES_ON", "SOL_EXCEPTIONS_SAFE_PROPAGATION", "SOL_LUAJIT=1")
