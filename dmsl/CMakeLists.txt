cmake_minimum_required(VERSION 3.25.0)
project(DMSL
        VERSION 1.0.1
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB sl_includes "include/*.hpp")
file(GLOB sl_src "src/*.cpp")

add_library(DMSL ${sl_includes} ${sl_src})
target_include_directories(DMSL PUBLIC include/)

target_link_libraries(DMSL PRIVATE AnterleEngine)

target_include_directories(DMSL PRIVATE AnterleEngine)
