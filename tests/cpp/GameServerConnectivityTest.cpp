/**
 * Copyright (c) DarkEmu
 * Basic connectivity test for the GameServer.
 */
#include "GameServer/GameServer.h"

#include <array>
#include <atomic>
#include <cerrno>
#include <chrono>
#include <cstring>
#include <iostream>
#include <thread>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace {

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

} // namespace

int main() {
    try {
        // Start the server in the background with a short polling loop.
        GameServer server(0);
        std::atomic_bool stop{false};
        std::thread server_thread([&] {
            while (!stop.load()) {
                server.RunOnce(50);
            }
        });

        uint16_t port = server.Port();
        if (port == 0) {
            std::cerr << "Failed to determine server port\n";
            stop.store(true);
            server_thread.join();
            return 1;
        }

        int fd = ::socket(AF_INET, SOCK_STREAM, 0);
        if (fd == -1) {
            std::cerr << "Socket creation failed\n";
            stop.store(true);
            server_thread.join();
            return 1;
        }

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

        if (::connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) {
            std::cerr << "Connect failed: " << std::strerror(errno) << '\n';
            ::close(fd);
            stop.store(true);
            server_thread.join();
            return 1;
        }

        // Send a small payload to verify the server receives data.
        std::array<uint8_t, 4> payload{0xC1, 0x04, 0xF1, 0x01};
        if (!sendAll(fd, payload.data(), payload.size())) {
            std::cerr << "Failed to send payload\n";
            ::close(fd);
            stop.store(true);
            server_thread.join();
            return 1;
        }

        // Allow the server loop to read and record the data.
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        ::close(fd);

        stop.store(true);
        server_thread.join();

        if (server.BytesReceived() == 0) {
            std::cerr << "Server did not receive any bytes\n";
            return 1;
        }

        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Test failed: " << ex.what() << '\n';
        return 1;
    }
}
