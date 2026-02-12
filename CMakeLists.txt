# Copyright (c) DarkEmu
# Build configuration and component wiring.
cmake_minimum_required(VERSION 3.22)
project(DarkEmu LANGUAGES CXX)

# Use a consistent C++20 toolchain and avoid compiler-specific extensions.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build shared common code and server modules.
# Main structure: server/ contains all server code (Connect, Game, common utilities)
# See docs/REORGANIZATION.md and docs/Structure.md for complete project organization
add_subdirectory(server/common)
add_subdirectory(server/Connect)
add_subdirectory(server/Game)

# Enable CTest and register the project's tests.
enable_testing()
add_subdirectory(server/tests)
