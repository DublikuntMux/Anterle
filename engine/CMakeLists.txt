cmake_minimum_required(VERSION 3.25.0)

project(AnterleEngine
        VERSION 1.0.3
        DESCRIPTION "3D game engine."
        LANGUAGES CXX
)

set(ENGINE_SOURCES 
    src/audio/audio_system.cpp
    src/backend/base.cpp
    src/helper/file.cpp
    src/object/component.cpp
    src/object/game_object.cpp
    src/object/object.cpp
    src/object/scene.cpp
    src/game.cpp

    include/audio/audio_system.hpp
    include/backend/base.hpp
    include/helper/file.hpp
    include/object/component.hpp
    include/object/game_object.hpp
    include/object/object.hpp
    include/object/scene.hpp
    include/game.hpp
)

option(USE_VULKAN "Use Vulkan backend" ON)
option(USE_DIRECTX "Use DirectX 12 backend" ON)

if(UNIX OR APPLE)
    set(USE_DIRECTX OFF)
endif()

if(USE_DIRECTX)
    list(APPEND ENGINE_SOURCES 
        src/backend/directx.cpp
        include/backend/directx.hpp
    )
endif()

if(USE_VULKAN)
    list(APPEND ENGINE_SOURCES 
        src/backend/vulkan.cpp
        include/backend/vulkan.hpp

        src/helper/vulkan.cpp
        include/helper/vulkan.hpp
    )
endif()

add_library(AnterleEngine ${ENGINE_SOURCES})
target_include_directories(AnterleEngine PUBLIC include/)

set_target_properties(AnterleEngine PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(AnterleEngine PROPERTIES SOVERSION 1)

include(FetchContent)

find_package(OpenMP REQUIRED)

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

if(USE_DIRECTX)
    FetchContent_Declare(
        directx
	    GIT_REPOSITORY https://github.com/microsoft/DirectX-Headers.git
	    GIT_TAG main
    )
    FetchContent_MakeAvailable(directx)
    target_link_libraries(AnterleEngine PUBLIC DirectX-Headers)
endif()

if(USE_VULKAN)
    find_package(Vulkan REQUIRED)
    target_link_libraries(AnterleEngine PUBLIC Vulkan::Vulkan)
endif()

target_link_libraries(AnterleEngine PUBLIC 
    uuid_v4::uuid_v4
    glm::glm
    glfw
    miniaudio
    imgui
    loguru::loguru
    tinyobjloader
)

target_include_directories(AnterleEngine PUBLIC 
    ${stb_SOURCE_DIR}
    ${freetype_SOURCE_DIR}/include
)
