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
    uint8_t UserTotal;    ///< Current load for the server list entry.
    uint8_t ListType;     ///< Protocol list type marker (0xCC by default).
    std::string Name;     ///< Human-readable server name for UI display.
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

    /// Load or reload server data using the default config file.
    void Load();
    /// Load server data from a JSON config file.
    bool LoadFromFile(const std::string& filename);
    /// Add a server entry manually (used by tests).
    void AddServer(uint16_t code, std::string name, std::string ip, uint16_t port, bool visible);
    /// Serialize the current server list into a packet buffer.
    void GetPacket(std::vector<uint8_t>& buffer) const;
    /// Find a server entry by its server code.
    const GameServerInfo* FindByCode(uint16_t serverCode) const;

private:
    /// Prevent external construction; use Instance() instead.
    ServerListManager() = default;

    /// In-memory list of servers that will be serialized.
    std::vector<GameServerInfo> servers_;
};

#endif // DARKEMU_SERVERLISTMANAGER_H
