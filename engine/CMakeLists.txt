cmake_minimum_required(VERSION 3.25.0)
project(AnterleEngine
        VERSION 1.0.3
        LANGUAGES CXX
)

file(GLOB_RECURSE engine_headers CONFIGURE_DEPENDS "./include/*.hpp")
file(GLOB_RECURSE engine_sources CONFIGURE_DEPENDS "./src/*.cpp")

add_library(AnterleEngine ${engine_headers} ${engine_sources})
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
