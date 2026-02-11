/*
 * Copyright (c) DarkEmu
 * Server list data structures and serialization interface.
 */

#ifndef DARKEMU_SERVERLISTMANAGER_H
#define DARKEMU_SERVERLISTMANAGER_H

#include <cstdint>
#include <string>
#include <vector>

// Runtime metadata for a single game server entry.
struct GameServerInfo {
    uint16_t ServerCode;  ///< Internal server identifier used in the protocol.
    uint8_t Percent;      ///< Population percent shown to the client.
    std::string IP;       ///< IP address where the game server listens.
    uint16_t Port;        ///< TCP port for the game server.
    bool Visible;         ///< Whether the server should be visible in the list.
};

/**
 * Singleton manager for the ConnectServer server list.
 * Provides loading and serialization into the server list packet.
 */
class ServerListManager {
public:
    /// Access the global instance of the server list manager.
    static ServerListManager* Instance();

    /// Load or reload server data (currently uses hardcoded entries).
    void Load();
    /// Serialize the current server list into a packet buffer.
    void GetPacket(std::vector<uint8_t>& buffer) const;

private:
    /// Prevent external construction; use Instance() instead.
    ServerListManager() = default;

    /// In-memory list of servers that will be serialized.
    std::vector<GameServerInfo> servers_;
};

#endif // DARKEMU_SERVERLISTMANAGER_H
