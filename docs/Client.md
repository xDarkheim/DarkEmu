<!--
Copyright (c) DarkEmu
Client connection guide for Main 5.2 to DarkEmu server.
-->
# Client Connection Guide

## Overview
The Main 5.2 client (MuOnline, Season 5, Update 2) connects to ConnectServer to retrieve the list of game servers.

## ConnectServer IP Address Location in Client

### Source Code (Source Main 5.2)
1. **File:** `source/LoginWin.cpp`  
   **Function:** `ConnectConnectionServer()`  
   **Line:** `CreateSocket(szServerIpAddress, g_ServerPort);`

2. **Variable Definitions:**
   - `szServerIpAddress` — global variable storing the ConnectServer IP address
   - `g_ServerPort` — global variable storing the ConnectServer TCP port (default: **44405**)

3. **Initialization:**
   - IP and port are typically loaded from a configuration file or hardcoded in the sources
   - MuOnline typically uses a config file read at client startup

### How to Change the IP Address

#### Option 1: Edit Source Code (if you have the sources)
Find in `source/WSclient.cpp` or the corresponding config:
```cpp
// Look for a line similar to:
char szServerIpAddress[32] = "127.0.0.1";  // or another IP
unsigned short g_ServerPort = 44405;
```

Replace with:
```cpp
char szServerIpAddress[32] = "localhost";  // or your IP address
unsigned short g_ServerPort = 44405;       // or your port
```

Then recompile the client.

#### Option 2: Use Configuration File
If the client supports a config file (typically `config.ini` or similar):
```ini
[SERVER]
ConnectServerIP=127.0.0.1
ConnectServerPort=44405
```

#### Option 3: Local Testing with Docker
If you run the ConnectServer Docker container:

**Host:**
```bash
docker compose up --build connectserver
```

**Client should connect to:**
- IP: `localhost` or `127.0.0.1` (if client is on the same host)
- IP: `<docker-host-ip>` or `<your-ip>` (if client is on a different machine)
- Port: `44405` (default)

## Connection Protocol

### Step 1: Server List Request
**Client sends:**
```
C1 04 F4 06
```
Where:
- `C1` — packet type (C1 = 1-byte length)
- `04` — packet size (4 bytes)
- `F4` — command code (ConnectServer)
- `06` — request type (server list request)

### Step 2: Server List Response
**Server sends:**
```
C2 <size:2> F4 06 <count:2> <server_entries...>
```

Where each entry is 4 bytes:
- `ServerCode` (2 bytes, little-endian)
- `UserTotal` (1 byte) — current server load
- `ListType` (1 byte, usually 0xCC)

### Step 3: Server Info Request
**Client sends:**
```
C1 06 F4 03 <server_code:2>
```

**Server sends:**
```
C1 22 F4 03 <server_ip:16> <server_port:2>
```

Where:
- `server_ip` — null-terminated IP address string (16 bytes)
- `server_port` (2 bytes, little-endian)

## Example: Connecting to Local DarkEmu Server

### 1. Start Docker Container
```bash
cd /home/darkheim/CLionProjects/DarkEmu
docker compose up --build connectserver
```

### 2. Verify Server is Listening on Port 44405
```bash
netstat -an | grep 44405
```
or
```bash
ss -tulpn | grep 44405
```

### 3. Configure Main 5.2 Client
Ensure the client is configured with:
- IP: `127.0.0.1` (or Docker host IP if client is on a different machine)
- Port: `44405`

### 4. Run the Client
```bash
# On Windows (if client is compiled)
Main.exe

# On Linux (if using Wine or other compatibility layer)
wine Main.exe
```

### 5. Check Server Logs
```bash
docker compose logs connectserver
```

Expected output:
```
ConnectServer listening on port 44405
```

## Configuration Files

### ServerList.json (DarkEmu)
Located at: `src/Servers/Connect/Data/ServerList.json`

Example:
```json
[
  {
    "code": 0,
    "name": "Test PVP",
    "ip": "127.0.0.1",
    "port": 55901,
    "user_total": 10,
    "visible": true
  },
  {
    "code": 20,
    "name": "Test VIP",
    "ip": "127.0.0.1",
    "port": 55919,
    "user_total": 5,
    "visible": true
  }
]
```

## Troubleshooting

### Check if ConnectServer is Running
```bash
curl -v telnet://127.0.0.1:44405
```

### View Docker Logs
```bash
docker compose logs -f connectserver
```

### Test Client (Console)
```bash
# Navigate to the project directory
cd /home/darkheim/CLionProjects/DarkEmu/cmake-build-debug/tests

# Run the connection test
./CS_ProtocolTest
```

## Additional Notes

- **Version:** Main 5.2, Season 5
- **Target Protocol:** MuOnline Season 5-6
- **Supported Protocols:** ConnectServer (F4:06, F4:03), GameServer (basic packet reception)
- **Encryption:** Not required for ConnectServer, optional for GameServer
- **Max Servers:** Unlimited (C2 format supports 2-byte length)

## Next Steps

1. Add JoinServer (F4:01, F4:02)
2. Add DataServer (database synchronization)
3. Implement logging and monitoring
4. Add configuration file for ports and IPs
5. Support hot-reload of ServerList.json

