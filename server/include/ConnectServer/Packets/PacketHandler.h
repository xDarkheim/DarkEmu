/**
 * Copyright (c) DarkEmu
 * ConnectServer packet dispatcher and response handlers.
 */

#ifndef DARKEMU_PACKETHANDLER_H
#define DARKEMU_PACKETHANDLER_H

#include <cstdint>
#include <span>

#include "Common/Network/Socket.h"

/**
 * Central packet handler for ConnectServer protocol messages.
 * Dispatches based on type/subtype and writes responses to the client socket.
 */
class PacketHandler {
public:
    /// Access the global PacketHandler instance.
    static PacketHandler* Instance();

    /// Dispatch a raw packet to the appropriate handler.
    void HandlePacket(Socket& client, std::span<const uint8_t> packet);

private:
    PacketHandler() = default;

    /// Handle the server list request (F4 06).
    void HandleServerList(Socket& client);
    /// Handle the server info request (F4 03).
    void HandleServerInfo(Socket& client, std::span<const uint8_t> packet);
    /// Send a complete response to the client, handling partial sends.
    void SendPacket(Socket& client, std::span<const uint8_t> payload);
};

#endif // DARKEMU_PACKETHANDLER_H
