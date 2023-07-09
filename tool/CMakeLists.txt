cmake_minimum_required(VERSION 3.25.0)
project(AnterleTool
        VERSION 1.0.3
        LANGUAGES CXX
)

file(GLOB_RECURSE tool_headers CONFIGURE_DEPENDS "./include/*.hpp")
file(GLOB_RECURSE tool_sources CONFIGURE_DEPENDS "./src/*.cpp")

add_executable(AnterleTool ${tool_headers} ${tool_sources})
target_include_directories(AnterleTool PUBLIC include/)

target_link_libraries(AnterleTool PRIVATE AnterleEngine)

target_include_directories(AnterleTool PRIVATE AnterleEngine)
