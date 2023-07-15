cmake_minimum_required(VERSION 3.25.0)

project(AnterleEngine
        VERSION 1.0.3
        LANGUAGES CXX
)

file(GLOB_RECURSE engine_headers CONFIGURE_DEPENDS "./include/*.hpp")
file(GLOB_RECURSE engine_sources CONFIGURE_DEPENDS "./src/*.cpp")

add_library(AnterleEngine ${engine_headers} ${engine_sources}  "src/object/ui/notify.cpp")
target_include_directories(AnterleEngine PUBLIC include/)

include(FetchContent)

FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG  master
)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
)
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
FetchContent_Declare(
    alpaca
    GIT_REPOSITORY https://github.com/p-ranav/alpaca.git
    GIT_TAG v0.2.1
)

FetchContent_Declare(
    loguru
	GIT_REPOSITORY https://github.com/emilk/loguru.git
	GIT_TAG master
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

set(LOGURU_WITH_STREAMS ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(stb)
FetchContent_MakeAvailable(glm)
FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(freetype)
FetchContent_MakeAvailable(alpaca)
FetchContent_MakeAvailable(loguru)

target_link_libraries(AnterleEngine PUBLIC 
    glad
    glm
    glfw
    imgui
    alpaca
    freetype
    loguru::loguru
)

target_include_directories(AnterleEngine PUBLIC 
    ${stb_SOURCE_DIR}
    ${freetype_SOURCE_DIR}/include
)
