cmake_minimum_required(VERSION 3.2)
CMAKE_POLICY(SET CMP0043 NEW)
project(ALIT-Engine VERSION 0.0.1 DESCRIPTION "Lit or not lit, that is the question.")

include_directories(${CMAKE_SOURCE_DIR})

add_definitions(-DAVLIT_ROOT="${CMAKE_SOURCE_DIR}/")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(Dependencies)
add_subdirectory(Core)
add_subdirectory(Applications)
