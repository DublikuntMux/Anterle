target("glad")
    set_kind("static")
    add_files("glad/src/**.c")
    add_includedirs("glad/include", {public = true})
