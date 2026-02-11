# DarkEmu

DarkEmu is a MuOnline server emulator in C++20. This repository targets the first Linux-native build of the server with a clean, modular architecture.

## Status
- First Linux-only server build in progress
- Common networking layer with sockets + epoll
- ConnectServer accepts connections and responds to the server list request
- GameServer accepts connections and logs incoming packets

## Documentation
- `docs/ConnectServer.md`
- `docs/GameServer.md`

## Build
```
cmake -S . -B build
cmake --build build
```

## Testing
```
ctest --test-dir build
```
