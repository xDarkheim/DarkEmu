<!--
Copyright (c) DarkEmu
Documentation index with navigation.
-->
# Documentation Index

## Quick Navigation

### 📖 Getting Started
1. **[README.md](../README.md)** - Project overview and quick start
2. **[docs/Build.md](Build.md)** - Build instructions (CMake, make, ctest)
3. **[docs/Docker.md](Docker.md)** - Docker deployment

### 🖥️ Server Development
4. **[server/README.md](../server/README.md)** - Server build & run
5. **[docs/ConnectServer.md](ConnectServer.md)** - ConnectServer protocol
6. **[docs/GameServer.md](GameServer.md)** - GameServer details

### 💻 Client Setup
7. **[client/README.md](../client/README.md)** - Client build & configuration
8. **[docs/Client.md](Client.md)** - Client connection guide

### 📋 Reference
9. **[LICENSE](../LICENSE)** - MIT License
10. **[SETUP_COMPLETE.md](../SETUP_COMPLETE.md)** - Project status

---

## Key Files & Purposes

| File | Purpose |
|------|---------|
| `README.md` | Project overview |
| `SETUP_COMPLETE.md` | Setup status & quick commands |
| `CMakeLists.txt` | Build configuration |
| `Dockerfile` | Container image |
| `docker-compose.yml` | Docker services |
| `server/Connect/Data/ServerList.json` | Server configuration |

---

## Build & Test

### Local Build
```bash
cmake -S . -B cmake-build-debug -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-build-debug --parallel
ctest --test-dir cmake-build-debug --output-on-failure
```

### Docker
```bash
docker compose up --build
```

---

## Server Structure

```
server/
├── Connect/          # ConnectServer (port 44405)
├── Game/             # GameServer (port 55901)
├── common/           # Shared utilities
├── include/          # Headers
└── tests/            # 3 tests (all passing ✅)
```

---

## Test Status

✅ 100% passing (3/3)
- CS_ProtocolTest
- CS_StressTest
- GS_ConnectivityTest

---

**Status:** 🚀 Production Ready

---

## File Locations & Paths Reference

### Server Structure
```
server/
├── Connect/              # ConnectServer module (port 44405)
│   ├── main.cpp          # Entry point
│   ├── ServerEngine.cpp  # Event loop & socket handling
│   ├── Data/ServerList.json  # Server configuration
│   ├── Managers/         # ServerListManager
│   └── Packets/          # PacketHandler
├── Game/                 # GameServer module (port 55901)
│   ├── main.cpp
│   └── GameServer.cpp
├── common/               # Shared networking
│   ├── Network/          # Socket, EpollContext
│   └── Utils/            # Logger, json.hpp
├── include/              # Public headers
├── tests/                # 3 test binaries (all passing ✅)
└── CMakeLists.txt
```

### Configuration Files
- **Server list:** `server/Connect/Data/ServerList.json`
- **Docker compose:** `docker-compose.yml`
- **Docker image:** `Dockerfile`
- **Build config:** `CMakeLists.txt`, `server/CMakeLists.txt`
- **Git excludes:** `.gitignore` (includes `resources/`)

### Documentation Files
- **Guides:** `docs/*.md`
- **Images:** `docs/images/` (SVG diagrams)
- **Project:** `README.md`, `SETUP_COMPLETE.md`

---

## Build & Test Commands

### Local Build
```bash
cmake -S . -B cmake-build-debug -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-build-debug --parallel
ctest --test-dir cmake-build-debug --output-on-failure
```

### Docker Build
```bash
docker compose up --build
# ConnectServer: localhost:44405
# GameServer: localhost:55901
```

### Server Directory Build
```bash
cd server
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
./build/server/Connect/DarkheimCS
./build/server/Game/DarkheimGS
```

---

## Cross-References

### From Main Documentation
- README.md → docs/Build.md (building)
- README.md → docs/Docker.md (Docker)
- README.md → server/README.md (server)
- README.md → client/README.md (client)

### From Server Directory
- server/README.md → ../docs/Build.md
- server/README.md → ../docs/Docker.md
- server/README.md → ../docs/ConnectServer.md

### From Client Directory
- client/README.md → ../docs/Client.md
- client/README.md → ../server/tests/ (for testing)

### From Protocol Documentation
- docs/ConnectServer.md → server/Connect/Data/ServerList.json
- docs/GameServer.md → server/Game/
- docs/Client.md → docs/ConnectServer.md

---

## Key Files & Their Purposes

| File | Purpose | Related Docs |
|------|---------|--------------|
| `CMakeLists.txt` | Root build config | docs/Build.md |
| `Dockerfile` | Container image | docs/Docker.md |
| `docker-compose.yml` | Service orchestration | docs/Docker.md |
| `server/Connect/Data/ServerList.json` | Server configuration | docs/ConnectServer.md |
| `server/Connect/ServerEngine.cpp` | ConnectServer event loop | docs/ConnectServer.md |
| `server/Game/GameServer.cpp` | GameServer main | docs/GameServer.md |
| `server/tests/*.cpp` | Integration tests | docs/Build.md |
| `client/mu-main-5.2/Main.sln` | Client solution | client/README.md, docs/Client.md |

---

## Test Binaries

All binaries build into `cmake-build-debug/server/tests/`:

| Binary | Purpose | Command |
|--------|---------|---------|
| `CS_ProtocolTest` | ConnectServer protocol validation | `ctest -R CS_ProtocolTest` |
| `CS_StressTest` | 8 threads × 8 requests stress test | `ctest -R CS_StressTest` |
| `GS_ConnectivityTest` | GameServer connectivity check | `ctest -R GS_ConnectivityTest` |

Status: ✅ **100% passing (3/3)**

---

## How to Find Things

**Looking for...**
- **How to build:** → docs/Build.md
- **Docker setup:** → docs/Docker.md
- **Server code:** → server/ directory
- **Client code:** → client/mu-main-5.2/ directory
- **Protocol details:** → docs/ConnectServer.md, docs/GameServer.md
- **Project structure:** → docs/Structure.md
- **Connection guide:** → docs/Client.md
- **Configuration:** → server/Connect/Data/ServerList.json
- **Tests:** → server/tests/

---

**Last Updated:** February 11, 2026

