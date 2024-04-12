project(Glad LANGUAGES C)

file(GLOB glad_src "src/*.c")

add_library(glad ${glad_src})

target_include_directories(glad PUBLIC include/)
