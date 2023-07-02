cmake_minimum_required(VERSION 3.25.0)
project(Anterle
        VERSION 1.0.3
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
	if (NOT ${COMPILER_IS_NVCC})
	  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic-errors")
	endif()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()

file(GLOB anterle_includes "include/*.hpp")
file(GLOB anterle_includes_folder "include/*/*.hpp")
file(GLOB anterle_src "src/*.cpp")
file(GLOB anterle_src_folder "src/*/*.cpp")

add_executable(Anterle ${anterle_includes} ${anterle_src} ${anterle_includes_folder} ${anterle_src_folder})
target_include_directories(Anterle PUBLIC include/)

add_subdirectory("deps")

find_package(Freetype REQUIRED)

target_link_libraries(Anterle PRIVATE 
    glfw
    glad
    imgui
    glm
    alpaca
    spdlog
    Freetype::Freetype
)

target_include_directories(Anterle PRIVATE
    Freetype::Freetype
)

add_custom_command(
        TARGET Anterle POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/resources
                ${CMAKE_CURRENT_BINARY_DIR}/resources)
