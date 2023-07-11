cmake_minimum_required(VERSION 3.25.0)
project(Imgui LANGUAGES CXX)

file(GLOB imgui_includes "include/*.h")
file(GLOB imgui_src "src/*.cpp")

add_library(imgui ${imgui_src} ${imgui_includes})

target_include_directories(imgui PUBLIC include/)
