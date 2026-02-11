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

bool setTimeouts(int fd, int timeoutMs) {
    timeval tv{};
    tv.tv_sec = timeoutMs / 1000;
    tv.tv_usec = (timeoutMs % 1000) * 1000;
    if (::setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) != 0) {
        return false;
    }
    return ::setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) == 0;
}

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

bool runClient(uint16_t port) {
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        return false;
    }

    setTimeouts(fd, 1000);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (::connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) {
        ::close(fd);
        return false;
    }

    std::array<uint8_t, 4> request{0xC1, 0x04, 0xF4, 0x06};
    if (!sendAll(fd, request.data(), request.size())) {
        ::close(fd);
        return false;
    }

    std::array<uint8_t, 8> response{};
    if (!recvExact(fd, response.data(), response.size())) {
        ::close(fd);
        return false;
    }

    std::array<uint8_t, 8> expected{0xC2, 0x08, 0xF4, 0x06, 0x01, 0x00, 0x01, 0x00};
    bool ok = response == expected;
    ::close(fd);
    return ok;
}

} // namespace

int main() {
    try {
        ServerEngine server(0);
        std::atomic_bool stop{false};
        std::thread server_thread([&] {
            while (!stop.load()) {
                server.runOnce(10);
            }
        });

        uint16_t port = server.port();
        if (port == 0) {
            std::cerr << "Failed to determine server port\n";
            stop.store(true);
            server_thread.join();
            return 1;
        }

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

        for (auto& t : clients) {
            t.join();
        }

        stop.store(true);
        server_thread.join();

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
