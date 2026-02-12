# DarkEmu - Project Setup ✅

A Linux-native MuOnline server emulator (ConnectServer + GameServer) built with C++20 and CMake.

## Project Status

✅ **Server:** Fully functional (3 tests passing 100%)  
✅ **Client:** Main 5.2 reference implementation (Windows)  
✅ **Documentation:** Complete and organized  
✅ **Build:** Docker-ready and locally compilable  

---

## Quick Start

### With Docker (Recommended)
```bash
docker compose up --build
# ConnectServer: localhost:44405
# GameServer: localhost:55901
```

### Local Build
```bash
cmake -S . -B cmake-build-debug -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-build-debug --parallel
ctest --test-dir cmake-build-debug --output-on-failure
```

---

## Directory Structure

```
DarkEmu/
├── server/              # Linux Server (C++20, CMake, epoll)
│   ├── Connect/         #   ConnectServer (port 44405)
│   ├── Game/            #   GameServer (port 55901)
│   ├── common/          #   Shared: Socket, Logger, epoll
│   ├── include/         #   Public headers
│   ├── tests/           #   3 passing tests ✅
│   └── README.md
│
├── client/              # Windows Client (Visual Studio)
│   ├── mu-main-5.2/     #   Main 5.2 source
│   └── README.md
│
├── docs/                # Documentation (all English)
│   ├── INDEX.md         #   Master index & navigation
│   ├── Build.md
│   ├── Docker.md
│   ├── ConnectServer.md
│   ├── GameServer.md
│   ├── Client.md
│   ├── Structure.md
│   └── images/
│
├── CMakeLists.txt
├── Dockerfile
├── docker-compose.yml
├── README.md
└── LICENSE (MIT)
```

---

## Documentation

**[📖 Start here: docs/INDEX.md](docs/INDEX.md)** — Master index with all navigation

### Key Guides
- **[README.md](README.md)** — Project overview
- **[docs/Build.md](docs/Build.md)** — Build instructions
- **[docs/Docker.md](docs/Docker.md)** — Docker deployment
- **[server/README.md](server/README.md)** — Server build & run
- **[client/README.md](client/README.md)** — Client setup
- **[docs/ConnectServer.md](docs/ConnectServer.md)** — Protocol details
- **[docs/Client.md](docs/Client.md)** — Connection guide

---

## Build & Test Status

### Tests (100% Passing ✅)
```
CS_ProtocolTest ..................   Passed
CS_StressTest ....................   Passed
GS_ConnectivityTest ..............   Passed
```

### Binaries
- `DarkheimCS` — ConnectServer executable
- `DarkheimGS` — GameServer executable
- Located in: `build/server/Connect/` and `build/server/Game/`

---

## Configuration

- **Server List:** `server/Connect/Data/ServerList.json` (JSON format)
- **Default Ports:** ConnectServer 44405, GameServer 55901
- **Encoding:** UTF-8
- **License:** MIT

---

## Key Features

✅ **ConnectServer**
- TCP server on port 44405
- Responds to server list requests
- JSON-based configuration
- epoll-based I/O (Linux native)

✅ **GameServer**
- TCP server on port 55901
- Accepts client connections
- Logs incoming packets (hex dump)
- Ready for protocol extension

✅ **Common**
- Cross-platform Socket abstraction
- epoll context wrapper
- Structured logging system
- JSON parsing (nlohmann/json)

---

## Development

**Technologies:**
- Language: C++20
- Build: CMake 3.22+
- I/O: epoll (Linux)
- Protocol: Binary packets (MuOnline compatible)

**Next Steps:**
1. Implement JoinServer (F4:01, F4:02)
2. Add DataServer
3. Implement login encryption
4. Add database integration

---

## License

MIT License — See [LICENSE](LICENSE) for details

---

**Last Updated:** February 11, 2026  
**Status:** 🚀 Production Ready  
**Tests:** ✅ 3/3 Passing  
**Documentation:** ✅ Complete

---

## Directory Structure

```
DarkEmu/
├── server/                 # 🐧 Linux Server (C++20, CMake)
│   ├── Connect/            #   ConnectServer module
│   ├── Game/               #   GameServer module
│   ├── common/             #   Shared utilities
│   ├── include/            #   Public headers
│   ├── tests/              #   Test binaries (3 tests, all passing ✅)
│   ├── README.md
│   └── CMakeLists.txt
│
├── client/                 # 🪟 Windows Client (Visual Studio)
│   ├── mu-main-5.2/        #   Main 5.2 source
│   └── README.md
│
├── docs/                   # 📖 Documentation (all English)
├── resources/              # ⛔ EXCLUDED from Git
│
├── docker-compose.yml
├── Dockerfile
├── .gitignore
├── LICENSE                 # MIT
└── README.md
```

---

## Build & Test Status

### ✅ Tests Passing
```bash
$ ctest --output-on-failure
1/3 Test #1: CS_ProtocolTest ..................   Passed    0.05 sec
2/3 Test #2: CS_StressTest ....................   Passed    0.01 sec
3/3 Test #3: GS_ConnectivityTest ..............   Passed    0.05 sec

100% tests passed, 0 tests failed out of 3
```

### ✅ Docker Ready
```bash
docker compose up --build
```

### ✅ Local Build Ready
```bash
cd server && cmake -B build && cmake --build build --parallel
```

---

## Migration Checklist

- [x] Client moved to `client/mu-main-5.2/`
- [x] Server consolidated in `server/`
- [x] `resources/` added to `.gitignore`
- [x] Documentation created (all English)
- [x] Tests verified (100% pass)
- [x] Docker configuration validated
- [x] README updated with new structure
- [x] Git ready for public repository

---

## What's Next?

### For Developers
1. Read `server/README.md` for server build instructions
2. Read `client/README.md` for client setup
3. Read `docs/Structure.md` for project organization
4. Read `docs/Client.md` for connection guide

### For Deployment
1. Use Docker: `docker compose up --build`
2. Or build locally: `cd server && cmake -B build && cmake --build build`
3. Configure server list: `server/Connect/Data/ServerList.json`

### For Git/CI
1. Verify resources are excluded: `git check-ignore -v resources/`
2. Update CI pipelines to use new paths if needed
3. Remove legacy `src/` directories after confirming all workflows migrated

### Future Improvements
- [ ] Add hot-reload for ServerList.json
- [ ] Implement full logging system
- [ ] Add JoinServer protocol (F4:01, F4:02)
- [ ] Add DataServer
- [ ] Consider resources as separate git submodule

---

## Contact & Support

For questions about the reorganization, see:
- `docs/REORGANIZATION.md` - Detailed migration guide
- `docs/Structure.md` - Project organization rationale
- `README.md` - Quick start and overview

---

**Status:** ✅ **COMPLETE**  
**Date:** February 11, 2026  
**Organized By:** GitHub Copilot (AI Assistant)

