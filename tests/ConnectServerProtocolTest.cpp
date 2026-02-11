#include "ConnectServer/ServerEngine.h"

#include <array>
#include <atomic>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <thread>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

namespace {

bool setRecvTimeout(int fd, int timeoutMs) {
    timeval tv{};
    tv.tv_sec = timeoutMs / 1000;
    tv.tv_usec = (timeoutMs % 1000) * 1000;
    return ::setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == 0;
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

} // namespace

int main() {
    try {
        ServerEngine server(0);
        std::atomic_bool stop{false};
        std::thread server_thread([&] {
            while (!stop.load()) {
                server.runOnce(50);
            }
        });

        uint16_t port = server.port();
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

        if (!setRecvTimeout(fd, 1000)) {
            std::cerr << "Failed to set receive timeout\n";
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

        std::array<uint8_t, 4> request{0xC1, 0x04, 0xF4, 0x06};
        if (!sendAll(fd, request.data(), request.size())) {
            std::cerr << "Failed to send request\n";
            ::close(fd);
            stop.store(true);
            server_thread.join();
            return 1;
        }

        std::array<uint8_t, 8> response{};
        if (!recvExact(fd, response.data(), response.size())) {
            std::cerr << "Failed to receive response\n";
            ::close(fd);
            stop.store(true);
            server_thread.join();
            return 1;
        }

        std::array<uint8_t, 8> expected{0xC2, 0x08, 0xF4, 0x06, 0x01, 0x00, 0x01, 0x00};
        if (response != expected) {
            std::cerr << "Unexpected response payload\n";
            ::close(fd);
            stop.store(true);
            server_thread.join();
            return 1;
        }

        ::close(fd);
        stop.store(true);
        server_thread.join();
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Test failed: " << ex.what() << '\n';
        return 1;
    }
}
