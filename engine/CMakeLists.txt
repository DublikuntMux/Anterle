cmake_minimum_required(VERSION 3.25.0)
project(AnterleEngine
        VERSION 1.0.3
        LANGUAGES CXX
)

file(GLOB engine_includes "include/*.hpp")
file(GLOB engine_includes_folder "include/*/*.hpp")
file(GLOB engine_src "src/*.cpp")
file(GLOB engine_src_folder "src/*/*.cpp")

add_library(AnterleEngine ${engine_includes} ${engine_src} ${engine_includes_folder} ${engine_src_folder})
target_include_directories(AnterleEngine PUBLIC include/)

find_package(Freetype REQUIRED)

target_link_libraries(AnterleEngine PUBLIC 
    glfw
    glad
    imgui
    glm
    alpaca
    plog
    Freetype::Freetype
)

target_include_directories(AnterleEngine PUBLIC 
    Freetype::Freetype
)
