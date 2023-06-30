cmake_minimum_required(VERSION 3.25.0)
project(Miniaudio LANGUAGES C)

file(GLOB miniaudio_includes "include/*.h")
file(GLOB miniaudio_src "src/*.c")

add_library(miniaudio ${miniaudio_src} ${miniaudio_includes})

target_include_directories(miniaudio PUBLIC include/)
