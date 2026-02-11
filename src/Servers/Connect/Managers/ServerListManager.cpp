/*
 * Copyright (c) DarkEmu
 * Server list manager implementation and packet serialization.
 */

#include "ConnectServer/Managers/ServerListManager.h"

#include <cstddef>

ServerListManager* ServerListManager::Instance() {
    // Function-local static keeps initialization thread-safe since C++11.
    static ServerListManager instance;
    return &instance;
}

void ServerListManager::Load() {
    // Reset current data before adding new entries.
    servers_.clear();
    // Dummy servers for development until loading from config is implemented.
    // Darkheim PVP server entry.
    servers_.push_back(GameServerInfo{0, 0, "127.0.0.1", 55901, true});
    // Darkheim VIP server entry.
    servers_.push_back(GameServerInfo{20, 0, "127.0.0.1", 55919, true});
}

void ServerListManager::GetPacket(std::vector<uint8_t>& buffer) const {
    // Packet layout: C2 <size> F4 06 followed by 4 bytes per server.
    const size_t count = servers_.size();
    const size_t size = 4 + (count * 4);

    // Prepare output buffer and serialize header first.
    buffer.clear();
    buffer.reserve(size);
    // Header: C2 <size> F4 06, then 4 bytes per server entry.
    buffer.push_back(0xC2);
    buffer.push_back(static_cast<uint8_t>(size));
    buffer.push_back(0xF4);
    buffer.push_back(0x06);

    // Serialize each server entry: code (LE), percent, visible flag.
    for (const auto& server : servers_) {
        buffer.push_back(static_cast<uint8_t>(server.ServerCode & 0xFF));
        buffer.push_back(static_cast<uint8_t>((server.ServerCode >> 8) & 0xFF));
        buffer.push_back(server.Percent);
        buffer.push_back(server.Visible ? 1 : 0);
    }
}
