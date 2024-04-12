project(
  AnterleEngine
  VERSION 1.0.3
  DESCRIPTION "Visual novel game engine"
  LANGUAGES CXX)

file(GLOB_RECURSE engine_sources CONFIGURE_DEPENDS "./src/*.cpp")

add_library(AnterleEngine ${engine_sources})
target_include_directories(AnterleEngine PUBLIC include/)

set_target_properties(AnterleEngine PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(AnterleEngine PROPERTIES SOVERSION 1)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG master)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4)
FetchContent_Declare(
  freetype
  GIT_REPOSITORY https://github.com/freetype/freetype.git
  GIT_TAG VER-2-13-2)

set(GLFW_BUILD_EXAMPLES
    OFF
    CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS
    OFF
    CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS
    OFF
    CACHE BOOL "" FORCE)
set(GLFW_INSTALL
    OFF
    CACHE BOOL "" FORCE)
set(GLFW_USE_HYBRID_HPG
    ON
    CACHE BOOL "" FORCE)
set(GLFW_BUILD_WAYLAND
    ON
    CACHE BOOL "" FORCE)

set(FT_DISABLE_BROTLI
    ON
    CACHE BOOL "" FORCE)
set(FT_DISABLE_BZIP2
    ON
    CACHE BOOL "" FORCE)
set(FT_DISABLE_PNG
    ON
    CACHE BOOL "" FORCE)
set(FT_DISABLE_ZLIB
    ON
    CACHE BOOL "" FORCE)
set(FT_DISABLE_HARFBUZZ
    ON
    CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glm)
FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(freetype)

target_link_libraries(
  AnterleEngine
  PUBLIC glad
         glm::glm
         glfw
         imgui
         freetype
         miniaudio
         fpng)

target_include_directories(AnterleEngine PUBLIC ${freetype_SOURCE_DIR}/include)
