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
```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

## Build
```
cmake --build build --parallel
```

## Test
```
ctest --test-dir build --output-on-failure
```

## Tips
- Use `-DCMAKE_BUILD_TYPE=Debug` for local debugging builds.
- `compile_commands.json` is generated in the build directory if needed by tooling.
