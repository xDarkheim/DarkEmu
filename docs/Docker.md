<!--
Copyright (c) DarkEmu
Docker usage instructions.
-->
# Docker

## Requirements
- Docker Engine
- Docker Compose plugin

## Build and run (ConnectServer + GameServer)
```zsh
docker compose up --build
```

## Run only one service
```zsh
docker compose up --build connectserver
```

```zsh
docker compose up --build gameserver
```

## Ports
- ConnectServer: 44405
- GameServer: 55901

## Notes
- The container copies `src/Servers/Connect/Data/ServerList.json` into the image.
- If you need custom server list data, mount a volume or rebuild the image.

