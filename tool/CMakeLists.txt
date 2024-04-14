project(
  AnterleTool
  VERSION 1.0.0
  LANGUAGES CXX)

file(GLOB_RECURSE tool_sources CONFIGURE_DEPENDS "./src/*.cpp")

add_executable(AnterleTool ${tool_sources})
target_include_directories(AnterleTool PUBLIC include/)

set_target_properties(AnterleTool PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(AnterleTool PROPERTIES SOVERSION 1)

FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG master)
FetchContent_Declare(
  glslang
  GIT_REPOSITORY https://github.com/KhronosGroup/glslang.git
  GIT_TAG main)

set(ENABLE_GLSLANG_BINARIES
    OFF
    CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(stb)
FetchContent_MakeAvailable(glslang)

target_link_libraries(AnterleTool PRIVATE AnterleEngine glslang SPIRV)
target_include_directories(AnterleTool PUBLIC ${stb_SOURCE_DIR} AnterleEngine)
