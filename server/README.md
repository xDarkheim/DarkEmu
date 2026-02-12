# DarkEmu Server Build Configuration
# This directory contains the server source and related infrastructure.

## Structure
- `Connect/` - ConnectServer module
- `Game/` - GameServer module  
- `common/` - Shared networking utilities
- `include/` - Public headers
- `tests/` - Server unit and integration tests
- `CMakeLists.txt` - Root CMake configuration

## Building
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

## Running
```bash
# ConnectServer
./build/server/Connect/DarkheimCS

# GameServer
./build/server/Game/DarkheimGS

# Or with Docker (from project root)
docker compose up --build
```

See `../docs/` for detailed documentation.

