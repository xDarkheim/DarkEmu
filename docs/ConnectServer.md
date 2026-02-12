<!--
Copyright (c) DarkEmu
ConnectServer protocol notes and packet layout.
-->
# ConnectServer

## Overview
ConnectServer handles the initial client handshake and serves the server list. It listens on TCP port 44405 by default and uses `epoll` for scalable event handling.

## Defaults
| Setting | Value |
| --- | --- |
| Listen port | 44405 |
| Config file | `server/Connect/Data/ServerList.json` |

## Packets

### Server List Request
```
C1 04 F4 06
```

### Server List Response
```
C2 <size:2> F4 06 <count:2> <servers>
```

`<size>` is the total packet size in 2 bytes (big-endian).  
`<count>` is the number of servers in 2 bytes (big-endian).  
`<servers>` is a list of 4-byte entries:
- Server code (2 bytes, little-endian)
- User total (1 byte) — current load percentage
- List type (1 byte, 0xCC = visible)

Packet size is `7 + (count * 4)` bytes.

## Layout
- Core engine: `server/Connect/`
- Server list manager: `server/Connect/Managers/`
- Packet handlers: `server/Connect/Packets/`
- Shared networking: `server/common/`

## Notes
- The server closes the client connection after responding.
- Server list entries are loaded from JSON on startup.
- See `server/Connect/Data/ServerList.json` for configuration format.
