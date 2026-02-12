/*
 * Copyright (c) DarkEmu
 * Server list manager implementation and packet serialization.
 */

#include "ConnectServer/Managers/ServerListManager.h"

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>

#include "Common/Utils/json.hpp"

ServerListManager* ServerListManager::Instance() {
    // Function-local static keeps initialization thread-safe since C++11.
    static ServerListManager instance;
    return &instance;
}

void ServerListManager::Load() {
    // Respect pre-seeded entries (useful for tests).
    if (!servers_.empty()) {
        return;
    }

    // Load the default config file from the ConnectServer data directory.
    if (!LoadFromFile("src/Servers/Connect/Data/ServerList.json")) {
        std::cerr << "ServerListManager: failed to load src/Servers/Connect/Data/ServerList.json\n";
    }
}

bool ServerListManager::LoadFromFile(const std::string& filename) {
    servers_.clear();

    std::filesystem::path path(filename);
    std::ifstream input(path);
    if (!input.is_open() && path.is_relative()) {
        // Walk up parent directories to locate the data file.
        std::filesystem::path base = std::filesystem::current_path();
        while (!base.empty()) {
            std::filesystem::path candidate = base / path;
            input.clear();
            input.open(candidate);
            if (input.is_open()) {
                path = candidate;
                break;
            }
            std::filesystem::path parent = base.parent_path();
            if (parent == base) {
                break;
            }
            base = parent;
        }
    }

    if (!input.is_open()) {
        std::cerr << "ServerListManager: unable to open config file: " << path << '\n';
        return false;
    }

    nlohmann::json root;
    try {
        input >> root;
    } catch (const nlohmann::json::parse_error& ex) {
        std::cerr << "ServerListManager: JSON parse error in " << path << ": " << ex.what() << '\n';
        return false;
    } catch (const nlohmann::json::exception& ex) {
        std::cerr << "ServerListManager: JSON error in " << path << ": " << ex.what() << '\n';
        return false;
    }

    if (!root.is_array()) {
        std::cerr << "ServerListManager: expected JSON array in " << path << '\n';
        return false;
    }

    size_t index = 0;
    for (const auto& entry : root) {
        if (!entry.is_object()) {
            std::cerr << "ServerListManager: entry " << index << " is not an object\n";
            ++index;
            continue;
        }

        if (!entry.contains("code") || !entry["code"].is_number_unsigned()) {
            std::cerr << "ServerListManager: entry " << index << " missing unsigned 'code'\n";
            ++index;
            continue;
        }
        if (!entry.contains("name") || !entry["name"].is_string()) {
            std::cerr << "ServerListManager: entry " << index << " missing string 'name'\n";
            ++index;
            continue;
        }
        if (!entry.contains("ip") || !entry["ip"].is_string()) {
            std::cerr << "ServerListManager: entry " << index << " missing string 'ip'\n";
            ++index;
            continue;
        }
        if (!entry.contains("port") || !entry["port"].is_number_unsigned()) {
            std::cerr << "ServerListManager: entry " << index << " missing unsigned 'port'\n";
            ++index;
            continue;
        }

        const uint32_t code_value = entry["code"].get<uint32_t>();
        if (code_value > std::numeric_limits<uint16_t>::max()) {
            std::cerr << "ServerListManager: entry " << index << " has out-of-range 'code'\n";
            ++index;
            continue;
        }

        const uint32_t port_value = entry["port"].get<uint32_t>();
        if (port_value > std::numeric_limits<uint16_t>::max()) {
            std::cerr << "ServerListManager: entry " << index << " has out-of-range 'port'\n";
            ++index;
            continue;
        }

        uint8_t user_total = 0;
        if (entry.contains("user_total")) {
            if (!entry["user_total"].is_number_unsigned()) {
                std::cerr << "ServerListManager: entry " << index << " has invalid 'user_total'\n";
                ++index;
                continue;
            }
            const uint32_t total_value = entry["user_total"].get<uint32_t>();
            if (total_value > std::numeric_limits<uint8_t>::max()) {
                std::cerr << "ServerListManager: entry " << index << " has out-of-range 'user_total'\n";
                ++index;
                continue;
            }
            user_total = static_cast<uint8_t>(total_value);
        } else if (entry.contains("percent")) {
            if (!entry["percent"].is_number_unsigned()) {
                std::cerr << "ServerListManager: entry " << index << " has invalid 'percent'\n";
                ++index;
                continue;
            }
            const uint32_t percent_value = entry["percent"].get<uint32_t>();
            if (percent_value > std::numeric_limits<uint8_t>::max()) {
                std::cerr << "ServerListManager: entry " << index << " has out-of-range 'percent'\n";
                ++index;
                continue;
            }
            user_total = static_cast<uint8_t>(percent_value);
        }

        uint8_t list_type = 0xCC;
        if (entry.contains("list_type")) {
            if (!entry["list_type"].is_number_unsigned()) {
                std::cerr << "ServerListManager: entry " << index << " has invalid 'list_type'\n";
                ++index;
                continue;
            }
            const uint32_t type_value = entry["list_type"].get<uint32_t>();
            if (type_value > std::numeric_limits<uint8_t>::max()) {
                std::cerr << "ServerListManager: entry " << index << " has out-of-range 'list_type'\n";
                ++index;
                continue;
            }
            list_type = static_cast<uint8_t>(type_value);
        }

        bool visible = true;
        if (entry.contains("visible")) {
            if (!entry["visible"].is_boolean()) {
                std::cerr << "ServerListManager: entry " << index << " has invalid 'visible'\n";
                ++index;
                continue;
            }
            visible = entry["visible"].get<bool>();
        }

        GameServerInfo info{};
        info.ServerCode = static_cast<uint16_t>(code_value);
        info.UserTotal = user_total;
        info.ListType = list_type;
        info.Name = entry["name"].get<std::string>();
        info.IP = entry["ip"].get<std::string>();
        info.Port = static_cast<uint16_t>(port_value);
        info.Visible = visible;
        servers_.push_back(std::move(info));
        ++index;
    }

    if (servers_.empty()) {
        std::cerr << "ServerListManager: no valid server entries loaded from " << path << '\n';
        return false;
    }

    return true;
}

void ServerListManager::AddServer(uint16_t code, std::string name, std::string ip, uint16_t port, bool visible) {
    GameServerInfo info{};
    info.ServerCode = code;
    info.UserTotal = 0;
    info.ListType = 0xCC;
    info.Name = std::move(name);
    info.IP = std::move(ip);
    info.Port = port;
    info.Visible = visible;
    servers_.push_back(std::move(info));
}

void ServerListManager::GetPacket(std::vector<uint8_t>& buffer) const {
    // Packet layout: C2 <size:2> F4 06 <count:2> followed by entries.
    uint16_t count = 0;
    for (const auto& server : servers_) {
        if (server.Visible) {
            ++count;
        }
    }

    const uint16_t size = static_cast<uint16_t>(7 + (count * 4));

    buffer.clear();
    buffer.reserve(size);

    buffer.push_back(0xC2);
    buffer.push_back(static_cast<uint8_t>((size >> 8) & 0xFF));
    buffer.push_back(static_cast<uint8_t>(size & 0xFF));
    buffer.push_back(0xF4);
    buffer.push_back(0x06);
    buffer.push_back(static_cast<uint8_t>((count >> 8) & 0xFF));
    buffer.push_back(static_cast<uint8_t>(count & 0xFF));

    for (const auto& server : servers_) {
        if (!server.Visible) {
            continue;
        }
        buffer.push_back(static_cast<uint8_t>(server.ServerCode & 0xFF));
        buffer.push_back(static_cast<uint8_t>((server.ServerCode >> 8) & 0xFF));
        buffer.push_back(server.UserTotal);
        buffer.push_back(server.ListType);
    }
}

const GameServerInfo* ServerListManager::FindByCode(uint16_t serverCode) const {
    // Linear search is sufficient for the small server list size.
    for (const auto& server : servers_) {
        if (server.ServerCode == serverCode) {
            return &server;
        }
    }
    return nullptr;
}
