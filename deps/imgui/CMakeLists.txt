cmake_minimum_required(VERSION 3.25.0)
project(Imgui LANGUAGES CXX)

file(GLOB imgui_includes "include/*.h")
file(GLOB imgui_src "src/*.cpp")

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)
FetchContent_Declare(
    freetype
    GIT_REPOSITORY https://github.com/freetype/freetype.git
    GIT_TAG VER-2-13-0
)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLFW_USE_HYBRID_HPG ON CACHE BOOL "" FORCE)
# set(GLFW_BUILD_WAYLAND ON CACHE BOOL "" FORCE)

set(FT_DISABLE_BROTLI ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BZIP2 ON CACHE BOOL "" FORCE)
set(FT_DISABLE_PNG ON CACHE BOOL "" FORCE)
set(FT_DISABLE_ZLIB ON CACHE BOOL "" FORCE)
set(FT_DISABLE_HARTBUZZ ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(freetype)
FetchContent_MakeAvailable(glfw)

add_library(imgui ${imgui_src} ${imgui_includes})

target_include_directories(imgui PUBLIC include/)


target_link_libraries(imgui PRIVATE 
    glfw
    freetype
)

target_include_directories(imgui PRIVATE 
    ${freetype_SOURCE_DIR}/include
)
