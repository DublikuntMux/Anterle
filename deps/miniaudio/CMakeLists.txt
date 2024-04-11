cmake_minimum_required(VERSION 3.25.0)
project(Miniaudio LANGUAGES C)

file(GLOB miniaudio_src "src/*.c")

add_library(miniaudio ${miniaudio_src})

target_include_directories(miniaudio PUBLIC include/)
