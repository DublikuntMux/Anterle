target("AnterleGame")
    set_kind("binary")
    set_version("1.0.2", {soname = true})
    add_deps("AnterleEngine")
    add_packages("glm", "glfw", "imgui", "miniaudio", "openrestry-luajit", "cpptrace", "mimalloc")
    add_files("src/**.cpp")
    add_installfiles("include/**.hpp")
    add_includedirs("include", {public = true})
    
    add_cflags("/W4", "/w14242", "/w14254", "/w14263", "/w14265", "/w14287", "/we4289", "/w14296", "/w14311", 
                "/w14545" ,"/w14546", "/w14547", "/w14549", "/w14555", "/w14619", "/w14640", "/w14826", "/w14905",
                "/w14906", "/w14928", "/permissive-", {tools={"cl", "clang_cl"}})
    add_cxxflags("/W4", "/w14242", "/w14254", "/w14263", "/w14265", "/w14287", "/we4289", "/w14296", "/w14311", 
                 "/w14545" ,"/w14546", "/w14547", "/w14549", "/w14555", "/w14619", "/w14640", "/w14826", "/w14905",
                 "/w14906", "/w14928", "/permissive-", {tools={"cl", "clang_cl"}})
    
    add_cflags("-Wall", "-Wextra", "-Wshadow", "-Wnon-virtual-dtor", "-Wold-style-cast", "-Wcast-align", 
               "-Wunused", "-Woverloaded-virtual", "-Wpedantic", "-Wconversion", "-Wsign-conversion",
               "-Wnull-dereference", "-Wdouble-promotion", "-Wformat=2", "-Wimplicit-fallthrough", 
               {tools={"clang", "gcc"}})
    add_cxxflags("-Wall", "-Wextra", "-Wshadow", "-Wnon-virtual-dtor", "-Wold-style-cast", "-Wcast-align", 
                 "-Wunused", "-Woverloaded-virtual", "-Wpedantic", "-Wconversion", "-Wsign-conversion",
                 "-Wnull-dereference", "-Wdouble-promotion", "-Wformat=2", "-Wimplicit-fallthrough", 
                 {tools={"clang", "gcc"}})
    
    add_cflags("-Wmisleading-indentation", "-Wduplicated-cond", "-Wduplicated-branches", "-Wlogical-op", 
               "-Wuseless-cast", {tools="gcc"})
    add_cxxflags("-Wmisleading-indentation", "-Wduplicated-cond", "-Wduplicated-branches", "-Wlogical-op", 
                 "-Wuseless-cast", {tools="gcc"})
