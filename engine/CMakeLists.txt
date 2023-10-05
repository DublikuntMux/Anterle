cmake_minimum_required(VERSION 3.25.0)

project(AnterleEngine
        VERSION 1.0.3
        DESCRIPTION "3D game engine"
        LANGUAGES CXX
)

file(GLOB_RECURSE engine_headers CONFIGURE_DEPENDS "./include/*.hpp")
file(GLOB_RECURSE engine_sources CONFIGURE_DEPENDS "./src/*.cpp")

add_library(AnterleEngine ${engine_headers} ${engine_sources})
target_include_directories(AnterleEngine PUBLIC include/)

set_target_properties(AnterleEngine PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(AnterleEngine PROPERTIES SOVERSION 1)

find_package(OpenMP REQUIRED)
find_package(Vulkan REQUIRED)

include(FetchContent)

FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG  master
)
FetchContent_Declare(
    uuid_v4
    GIT_REPOSITORY https://github.com/crashoz/uuid_v4.git
    GIT_TAG  master
)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
)
FetchContent_Declare(
    tinyobjloader
    GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
    GIT_TAG release
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
set(GLFW_BUILD_WAYLAND ON CACHE BOOL "" FORCE)

set(LOGURU_WITH_STREAMS ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(stb)
FetchContent_MakeAvailable(glm)
FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(uuid_v4)
FetchContent_MakeAvailable(loguru)
FetchContent_MakeAvailable(tinyobjloader)

target_link_libraries(AnterleEngine PUBLIC 
    Vulkan::Vulkan
    uuid_v4::uuid_v4
    glm::glm
    glfw
    miniaudio
    loguru::loguru
    tinyobjloader
)

target_include_directories(AnterleEngine PUBLIC 
    ${stb_SOURCE_DIR}
    ${freetype_SOURCE_DIR}/include
)
