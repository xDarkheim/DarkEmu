<div align="center">

# DarkEmu
MuOnline server emulator with clean, modular networking layers for Linux.

See [`docs/INDEX.md`](docs/INDEX.md) for documentation.

<picture>
  <source media="(prefers-color-scheme: dark)" srcset="docs/images/banner.svg">
  <source media="(prefers-color-scheme: light)" srcset="docs/images/banner-light.svg">
  <img src="docs/images/banner-light.svg" alt="DarkEmu banner" width="840">
</picture>

[![CI](https://img.shields.io/github/actions/workflow/status/xDarkheim/DarkEmu/ci.yml?branch=main&style=for-the-badge&logo=githubactions&logoColor=white)](https://github.com/xDarkheim/DarkEmu/actions/workflows/ci.yml)
![C++](https://img.shields.io/badge/C%2B%2B-Modern-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-3.22%2B-064F8C?style=for-the-badge&logo=cmake&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-yes-FCC624?style=for-the-badge&logo=linux&logoColor=black)

</div>

## Project Structure
```
DarkEmu/
├── server/              # Linux server implementation (C++, CMake)
│   ├── Connect/         # ConnectServer module
│   ├── Game/            # GameServer module
│   ├── common/          # Shared utilities (networking, logging)
│   ├── include/         # Public headers
│   ├── tests/           # Server tests
│   └── CMakeLists.txt
├── client/              # Client reference (Windows)
│   └── mu-main-5.2/     # Main 5.2 source (Visual Studio project)
├── docs/                # Documentation
├── LICENSE              # MIT License
└── docker-compose.yml   # Docker orchestration
```

## Modules
| Module | Role |
| --- | --- |
| ConnectServer | Handles initial handshake and serves game server list. |
| GameServer | Accepts TCP clients and logs incoming packets for testing. |
| Common | Shared socket (epoll), networking, and logging utilities. |

## Quick Start
```bash
# Build and run server with Docker
docker compose up --build

# Or build locally
cd server && cmake -B build && cmake --build build --parallel
```

## Configuration
- Server list: `server/Connect/Data/ServerList.json`
- Default ports: ConnectServer 44405, GameServer 55901

## Status
- Server foundations in progress.
- ConnectServer responds to server list requests.
- GameServer accepts connections and logs incoming packets.

## Documentation

Start with **[docs/INDEX.md](docs/INDEX.md)** for navigation:

- `docs/Build.md` — Build instructions
- `docs/Docker.md` — Docker deployment
- `docs/ConnectServer.md` — Protocol details
- `docs/GameServer.md` — Server behavior
- `docs/Client.md` — Client connection guide
- `server/README.md` — Server build & run
- `client/README.md` — Client setup
- `LICENSE` — MIT license

## Visuals
<picture>
  <source media="(prefers-color-scheme: dark)" srcset="docs/images/modules.svg">
  <source media="(prefers-color-scheme: light)" srcset="docs/images/modules-light.svg">
  <img src="docs/images/modules-light.svg" alt="Modules Overview">
</picture>

<picture>
  <source media="(prefers-color-scheme: dark)" srcset="docs/images/connectserver-flow.svg">
  <source media="(prefers-color-scheme: light)" srcset="docs/images/connectserver-flow-light.svg">
  <img src="docs/images/connectserver-flow-light.svg" alt="ConnectServer Flow">
</picture>
