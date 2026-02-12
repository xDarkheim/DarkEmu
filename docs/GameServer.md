<!--
Copyright (c) DarkEmu
GameServer module notes and startup behavior.
-->
# GameServer

## Overview
GameServer accepts TCP clients and logs the first packets it receives. At this stage it does not decrypt or parse the login request; it only prints a hex dump to verify connectivity.

## Defaults
| Setting | Value |
| --- | --- |
| Listen port | 55901 |

## Behavior
- Accepts new connections with `epoll`.
- Receives inbound data and prints a hex dump via `Log::Info`.
- Does not respond to clients yet.

## Layout
- Core engine: `server/Game/`
- Shared networking: `server/common/`

## Notes
- Encrypted login packets are expected immediately after connect.
- Packet parsing and decryption will be added later.
