<div align="center">

# DarkEmu
MuOnline server emulator with clean, modular networking layers for Linux.

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

## Modules
| Module | Role |
| --- | --- |
| ConnectServer | Handles the initial handshake and server list responses. |
| GameServer | Accepts clients and logs the first encrypted packets for validation. |
| Common | Shared socket, epoll, and logging utilities. |

## Configuration
- Server list data lives in `src/Servers/Connect/Data/ServerList.json`.

## Status
- Server foundations in progress.
- ConnectServer responds to server list requests.
- GameServer accepts connections and logs incoming packets.

## Documentation
- `docs/Build.md` - build and test steps
- `docs/ConnectServer.md` - protocol and layout notes
- `docs/GameServer.md` - behavior and defaults

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
