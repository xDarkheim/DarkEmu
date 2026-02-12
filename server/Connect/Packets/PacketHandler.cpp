/**
 * Copyright (c) DarkEmu
 * ConnectServer packet dispatcher and response handlers.
 */
#include "ConnectServer/Packets/PacketHandler.h"

#include <algorithm>
#include <array>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <vector>

#include "ConnectServer/Managers/ServerListManager.h"

PacketHandler* PacketHandler::Instance() {
    // Function-local static keeps initialization thread-safe since C++11.
    static PacketHandler instance;
    return &instance;
}

void PacketHandler::HandlePacket(Socket& client, std::span<const uint8_t> packet) {
    // Require a minimal header before parsing.
    if (packet.size() < 4) {
        return;
    }

    // Only handle standard C1 packets for now.
    if (packet[0] != 0xC1) {
        return;
    }

    // Dispatch only ConnectServer (F4) packets.
    if (packet[2] != 0xF4) {
        return;
    }

    // Switch on the subtype byte.
    switch (packet[3]) {
        case 0x06:
            HandleServerList(client);
            break;
        case 0x03:
            HandleServerInfo(client, packet);
            break;
        default:
            break;
    }
}

void PacketHandler::HandleServerList(Socket& client) {
    // Build the server list response from the current manager data.
    std::vector<uint8_t> response;
    ServerListManager::Instance()->GetPacket(response);
    SendPacket(client, response);
}

void PacketHandler::HandleServerInfo(Socket& client, std::span<const uint8_t> packet) {
    // Ensure the packet contains the 2-byte server id.
    if (packet.size() < 6) {
        return;
    }

    // Extract server id as little-endian (low byte first).
    uint16_t server_id = static_cast<uint16_t>(packet[4])
        | (static_cast<uint16_t>(packet[5]) << 8);

    // Look up the requested server in the server list manager.
    const GameServerInfo* info = ServerListManager::Instance()->FindByCode(server_id);
    if (info == nullptr) {
        return;
    }

    // Build response: C1 <size> F4 03 [IP:16 bytes] [Port:2 bytes].
    std::vector<uint8_t> response;
    response.reserve(4 + 16 + 2);
    response.push_back(0xC1);
    response.push_back(static_cast<uint8_t>(4 + 16 + 2));
    response.push_back(0xF4);
    response.push_back(0x03);

    // Write a null-terminated IP string, padded to 16 bytes.
    std::array<uint8_t, 16> ip_bytes{};
    const size_t copy_len = std::min(info->IP.size(), ip_bytes.size() - 1);
    std::memcpy(ip_bytes.data(), info->IP.data(), copy_len);
    response.insert(response.end(), ip_bytes.begin(), ip_bytes.end());

    // Append the port as little-endian (low byte first).
    response.push_back(static_cast<uint8_t>(info->Port & 0xFF));
    response.push_back(static_cast<uint8_t>((info->Port >> 8) & 0xFF));

    SendPacket(client, response);
}

void PacketHandler::SendPacket(Socket& client, std::span<const uint8_t> payload) {
    // Send the payload, handling partial writes on non-blocking sockets.
    size_t offset = 0;
    while (offset < payload.size()) {
        ssize_t sent = client.send(payload.subspan(offset));
        if (sent > 0) {
            offset += static_cast<size_t>(sent);
            continue;
        }
        if (sent == 0) {
            break;
        }
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            break;
        }
        std::cerr << "send error: " << std::strerror(errno) << '\n';
        break;
    }
}
