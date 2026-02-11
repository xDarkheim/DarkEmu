# DarkEmu

DarkEmu is a MuOnline server emulator in C++20. This repository targets the first Linux-native build of the server, starting with the ConnectServer module and a clean, modular architecture.

## Status
- First Linux-only server build in progress
- Common networking layer with sockets + epoll
- ConnectServer accepts connections and responds to the server list request

## Documentation
- `docs/ConnectServer.md`

## Build
```
cmake -S . -B build
cmake --build build
```

## Testing
```
ctest --test-dir build
```
