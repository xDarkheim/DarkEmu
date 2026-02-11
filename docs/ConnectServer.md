<!--
Copyright (c) DarkEmu
ConnectServer protocol notes and packet layout.
-->
# ConnectServer

The ConnectServer module handles the initial client handshake and serves the server list. It listens on TCP port 44405 by default and uses `epoll` for scalable event handling.

## Protocol

Server List Request:
```
C1 04 F4 06
```

Server List Response:
```
C2 <size> F4 06 <servers>
```

`<size>` is the total packet size in bytes.

`<servers>` is a list of 4-byte entries:
- Server code (2 bytes, little-endian)
- Percent (1 byte)
- Visible (1 byte, 1 = visible)

Packet size is `4 + (count * 4)`.

## Notes
- The implementation closes the client connection after responding.
- The networking layer lives in `src/Common`.
- The server list manager lives in `src/Servers/Connect/Managers`.
- The server list configuration lives in `src/Servers/Connect/Data/ServerList.json`.
