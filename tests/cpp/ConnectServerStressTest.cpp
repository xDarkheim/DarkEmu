/**
 * Copyright (c) DarkEmu
 * Stress test for concurrent ConnectServer server list requests.
 */

#include "ConnectServer/Managers/ServerListManager.h"
#include "ConnectServer/ServerEngine.h"

#include <array>
#include <atomic>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <thread>
#include <vector>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

namespace {

// Configure send/receive timeouts in milliseconds.
bool setTimeouts(int fd, int timeoutMs) {
    timeval tv{};
    tv.tv_sec = timeoutMs / 1000;
    tv.tv_usec = (timeoutMs % 1000) * 1000;
    if (::setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) != 0) {
        return false;
    }
    return ::setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) == 0;
}

// Send all bytes, retrying on EINTR.
bool sendAll(int fd, const uint8_t* data, size_t size) {
    size_t offset = 0;
    while (offset < size) {
        ssize_t sent = ::send(fd, data + offset, size - offset, 0);
        if (sent > 0) {
            offset += static_cast<size_t>(sent);
            continue;
        }
        if (sent == -1 && errno == EINTR) {
            continue;
        }
        return false;
    }
    return true;
}

// Receive an exact number of bytes, retrying on EINTR.
bool recvExact(int fd, uint8_t* data, size_t size) {
    size_t offset = 0;
    while (offset < size) {
        ssize_t received = ::recv(fd, data + offset, size - offset, 0);
        if (received > 0) {
            offset += static_cast<size_t>(received);
            continue;
        }
        if (received == 0) {
            return false;
        }
        if (errno == EINTR) {
            continue;
        }
        return false;
    }
    return true;
}

// Execute one complete request/response cycle.
bool runClient(uint16_t port) {
    // Open a client socket to the ConnectServer.
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        return false;
    }

    // Apply short timeouts to avoid long stalls.
    setTimeouts(fd, 1000);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    // Connect to the server.
    if (::connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) {
        ::close(fd);
        return false;
    }

    // Send the server list request.
    std::array<uint8_t, 4> request{0xC1, 0x04, 0xF4, 0x06};
    if (!sendAll(fd, request.data(), request.size())) {
        ::close(fd);
        return false;
    }

    // Receive the full server list response.
    std::array<uint8_t, 12> response{};
    if (!recvExact(fd, response.data(), response.size())) {
        ::close(fd);
        return false;
    }

    std::array<uint8_t, 12> expected{
        0xC2, 0x0C, 0xF4, 0x06,
        0x00, 0x00, 0x00, 0x01,
        0x14, 0x00, 0x00, 0x01
    };
    // Compare the response to the expected payload.
    bool ok = response == expected;
    ::close(fd);
    return ok;
}

} // namespace

int main() {
    try {
        // Seed the server list to avoid external config dependencies.
        ServerListManager::Instance()->AddServer(0, "Test PVP", "127.0.0.1", 55901, true);
        ServerListManager::Instance()->AddServer(20, "Test VIP", "127.0.0.1", 55919, true);

        // Start the server in the background with a tight polling loop.
        ServerEngine server(0);
        std::atomic_bool stop{false};
        std::thread server_thread([&] {
            while (!stop.load()) {
                server.runOnce(10);
            }
        });

        // Capture the bound port for client connections.
        uint16_t port = server.port();
        if (port == 0) {
            std::cerr << "Failed to determine server port\n";
            stop.store(true);
            server_thread.join();
            return 1;
        }

        // Launch multiple clients in parallel.
        constexpr int kThreads = 8;
        constexpr int kIterations = 8;
        std::atomic_int failures{0};
        std::vector<std::thread> clients;
        clients.reserve(kThreads);

        for (int i = 0; i < kThreads; ++i) {
            clients.emplace_back([&] {
                for (int j = 0; j < kIterations; ++j) {
                    if (!runClient(port)) {
                        failures.fetch_add(1);
                        return;
                    }
                }
            });
        }

        // Wait for all clients to finish.
        for (auto& t : clients) {
            t.join();
        }

        // Shut down the server loop.
        stop.store(true);
        server_thread.join();

        // Report any failures detected by the client threads.
        if (failures.load() != 0) {
            std::cerr << "Stress test failed for " << failures.load() << " client(s)\n";
            return 1;
        }
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Stress test failed: " << ex.what() << '\n';
        return 1;
    }
}
