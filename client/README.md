# DarkEmu Client - Main 5.2
# MuOnline Season 5, Update 2

## Overview
This directory contains the client source code for Main 5.2 compiled under Windows (Visual Studio).
The client is a reference implementation used to test server compatibility.

## Structure
- `source/` - Client source code (C++)
- `bin/` - Runtime files and resources
- `dependencies/` - Third-party libraries
- `Global Release/` - Release build artifacts
- `Main.sln` - Visual Studio solution
- `Main.vcxproj` - Visual Studio project

## Building
1. Open `Main.sln` in Visual Studio 2019 or later
2. Build the solution (Release configuration recommended)
3. Output: `Global Release/Main.exe`

## Configuration
Edit server IP and port in:
- `source/WSclient.cpp` - `szServerIpAddress`, `g_ServerPort` variables
- Or use external config file (if client supports it)

## Connecting to DarkEmu
Default settings:
- ConnectServer IP: `127.0.0.1`
- ConnectServer Port: `44405`
- GameServer IP: `127.0.0.1`
- GameServer Port: `55901`

See `../docs/Client.md` for detailed connection guide.

## Notes
- Windows-only build (requires Visual Studio)
- C++ standard: Varies (originally MSVC)
- Target platform: x86/x64 Windows

For Linux server testing, use the console tests in `../server/tests/`.

