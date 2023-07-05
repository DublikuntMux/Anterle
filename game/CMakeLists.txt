cmake_minimum_required(VERSION 3.25.0)
project(AnterleGame
        VERSION 1.0.3
        LANGUAGES CXX
)

file(GLOB game_includes "include/*.hpp")
file(GLOB game_includes_folder "include/*/*.hpp")
file(GLOB game_src "src/*.cpp")
file(GLOB game_src_folder "src/*/*.cpp")

add_executable(AnterleGame ${game_includes} ${game_src} ${game_includes_folder} ${game_src_folder})
target_include_directories(AnterleGame PUBLIC include/)

target_link_libraries(AnterleGame PRIVATE AnterleEngine)

target_include_directories(AnterleGame PRIVATE AnterleEngine)

add_custom_command(
        TARGET AnterleGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/Game/resources
                ${CMAKE_CURRENT_BINARY_DIR}/resources
)
