cmake_minimum_required(VERSION 3.25.0)
project(Anterle
        VERSION 1.0.1
        LANGUAGES CXX
)

include("cmake/Cache.cmake")
include("cmake/CompilerWarnings.cmake")
include("cmake/StaticAnalyzers.cmake")
include("cmake/InterproceduralOptimization.cmake") 

option(ANTERLE_ENABLE_IPO "Enable IPO/LTO" OFF)
option(ANTERLE_ENABLE_CACHE "Enable ccache" OFF)
option(ANTERLE_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
option(ANTERLE_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
option(ANTERLE_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
option(ANTERLE_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
option(ANTERLE_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
option(ANTERLE_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
option(ANTERLE_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)

if (NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
endif()

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

if(ANTERLE_ENABLE_IPO)
    enable_ipo()
endif()

if(ANTERLE_ENABLE_CACHE)
    enable_cache()
endif()

add_subdirectory("dmsl")
add_subdirectory("deps")
add_subdirectory("engine")
add_subdirectory("game")

if(ANTERLE_ENABLE_CLANG_TIDY)
    enable_clang_tidy(AnterleEngine ${ANTERLE_WARNINGS_AS_ERRORS})
endif()

if(ANTERLE_ENABLE_CLANG_TIDY)
    enable_clang_tidy(AnterleGame ${ANTERLE_WARNINGS_AS_ERRORS})
endif()

set_project_warnings(
    AnterleEngine
    ${ANTERLE_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
)
set_project_warnings(
    AnterleGame
    ${ANTERLE_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
)

enable_sanitizers(
    AnterleEngine
    ${ANTERLE_ENABLE_SANITIZER_ADDRESS}
    ${ANTERLE_ENABLE_SANITIZER_LEAK}
    ${ANTERLE_ENABLE_SANITIZER_UNDEFINED}
    ${ANTERLE_ENABLE_SANITIZER_THREAD}
    ${ANTERLE_ENABLE_SANITIZER_MEMORY}
)
enable_sanitizers(
    AnterleGame
    ${ANTERLE_ENABLE_SANITIZER_ADDRESS}
    ${ANTERLE_ENABLE_SANITIZER_LEAK}
    ${ANTERLE_ENABLE_SANITIZER_UNDEFINED}
    ${ANTERLE_ENABLE_SANITIZER_THREAD}
    ${ANTERLE_ENABLE_SANITIZER_MEMORY}
)
