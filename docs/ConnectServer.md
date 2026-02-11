# ConnectServer

The ConnectServer module handles the initial client handshake and serves the server list. It listens on TCP port 44405 by default and uses `epoll` for scalable event handling.

## Protocol

Server List Request:
```
C1 04 F4 06
```

Server List Response:
```
C2 <size> F4 06 01 00 01 00
```

`<size>` is the total packet size in bytes. The current response length is 8 bytes.

## Notes
- The implementation closes the client connection after responding.
- The networking layer lives in `src/common`.
