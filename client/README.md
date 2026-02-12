# DarkEmu Client

Reference client source (Main 5.2, MuOnline Season 5).

## Building

The client source is Windows-only and cannot be compiled on Linux natively.

**See [`../docs/Building-Client.md`](../docs/Building-Client.md) for options:**

1. **Wine** - Run pre-compiled Windows binary
2. **MinGW** - Cross-compile from Linux to Windows
3. **Native Linux** - Rewrite with cross-platform libraries (future)

## Configuration

To change server connection, edit:
- `source/WSclient.cpp` - Update `szServerIpAddress` and `g_ServerPort`

Default:
- ConnectServer: `127.0.0.1:44405`
- GameServer: `127.0.0.1:55901`

See `../docs/Client.md` for details.

## Testing

Server can be tested without the GUI client using console tests:

```bash
ctest --test-dir ../cmake-build-debug --output-on-failure
```

