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

add_subdirectory("deps")
add_subdirectory("engine")
add_subdirectory("game")
