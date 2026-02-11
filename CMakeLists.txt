cmake_minimum_required(VERSION 4.1)
project(DarkheimMu LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src/Common)
add_subdirectory(src/Servers/connect)

enable_testing()
add_subdirectory(tests)
