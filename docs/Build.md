<!--
Copyright (c) DarkEmu
Build and test instructions.
-->
# Build

## Requirements
- Linux
- CMake 3.22+
- C++20-capable compiler (GCC or Clang)

## Configure
```bash
# Build from project root
cmake -S . -B cmake-build-debug -DCMAKE_BUILD_TYPE=Release

# Or build from server directory
cd server && cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

## Build
```bash
# From project root
cmake --build cmake-build-debug --parallel

# Or from server directory
cmake --build build --parallel
```

## Test
```bash
# From project root
ctest --test-dir cmake-build-debug --output-on-failure

# Or from server directory
ctest --test-dir build --output-on-failure
```

## Tips
- Use `-DCMAKE_BUILD_TYPE=Debug` for local debugging builds.
- `compile_commands.json` is generated in the build directory if needed by tooling.
