cmake_minimum_required(VERSION 3.25.0)
project(AnterleGame
        VERSION 1.0.3
        LANGUAGES CXX
)

file(GLOB_RECURSE game_headers CONFIGURE_DEPENDS "./include/*.hpp")
file(GLOB_RECURSE game_sources CONFIGURE_DEPENDS "./src/*.cpp")

add_executable(AnterleGame ${game_headers} ${game_sources})
target_include_directories(AnterleGame PUBLIC include/)

target_link_libraries(AnterleGame PRIVATE AnterleEngine)

target_include_directories(AnterleGame PRIVATE AnterleEngine)

add_custom_command(
        TARGET AnterleGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/Game/resources
                ${CMAKE_CURRENT_BINARY_DIR}/resources
)
