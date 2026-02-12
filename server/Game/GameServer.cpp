/**
 * Copyright (c) DarkEmu
 * GameServer event loop and packet logging.
 */

#include "GameServer/GameServer.h"

#include "Common/Utils/Logger.h"

#include <array>
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <iomanip>
#include <netinet/in.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>

GameServer::GameServer(uint16_t port) : events_(64), port_(port) {
    // Create and configure the listening socket.
    listen_socket_ = Socket::createTcp();
    listen_socket_.setNonBlocking(true);
    listen_socket_.bind(port_);
    listen_socket_.listen();

    // Read back the bound port (supports ephemeral port 0 in tests).
    sockaddr_in addr{};
    socklen_t addr_len = sizeof(addr);
    if (::getsockname(listen_socket_.fd(), reinterpret_cast<sockaddr*>(&addr), &addr_len) == -1) {
        throw std::runtime_error(std::strerror(errno));
    }
    port_ = ntohs(addr.sin_port);

    // Register the listening socket for read events.
    epoll_.add(listen_socket_.fd(), EPOLLIN);
}

void GameServer::Run() {
    // Main event loop: wait for epoll events and dispatch them.
    Log::Info("GameServer listening on port " + std::to_string(port_));
    while (true) {
        RunOnce(-1);
    }
}

void GameServer::RunOnce(int timeoutMs) {
    // Single iteration of the event loop (used by tests).
    int ready = epoll_.wait(events_, timeoutMs);
    for (int i = 0; i < ready; ++i) {
        const epoll_event& ev = events_[i];
        int fd = ev.data.fd;
        if (fd == listen_socket_.fd()) {
            // Accept new connections from the listening socket.
            if (ev.events & EPOLLIN) {
                HandleAccept();
            }
            continue;
        }
        // Close clients on error or hang-up events.
        if (ev.events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
            CloseClient(fd);
            continue;
        }
        // Read available data from connected clients.
        if (ev.events & EPOLLIN) {
            HandleRead(fd);
        }
    }
}

uint16_t GameServer::Port() const noexcept {
    return port_;
}

size_t GameServer::BytesReceived() const noexcept {
    return total_bytes_received_;
}

void GameServer::HandleAccept() {
    // Drain all pending accept calls until the listen socket would block.
    while (true) {
        Socket client = listen_socket_.accept();
        if (!client.isValid()) {
            break;
        }
        client.setNonBlocking(true);
        int fd = client.fd();
        // Register the new client for read and hang-up events.
        epoll_.add(fd, EPOLLIN | EPOLLRDHUP);
        clients_.emplace(fd, ClientState{std::move(client), {}});
    }
}

void GameServer::HandleRead(int fd) {
    auto it = clients_.find(fd);
    if (it == clients_.end()) {
        return;
    }

    ClientState& client = it->second;
    std::array<uint8_t, 1024> temp{};
    // Read until the socket would block or closes.
    while (true) {
        ssize_t bytes = client.socket.recv(temp);
        if (bytes > 0) {
            // Append bytes to the client buffer and log the new data.
            client.buffer.insert(client.buffer.end(), temp.begin(), temp.begin() + static_cast<size_t>(bytes));
            total_bytes_received_ += static_cast<size_t>(bytes);
            LogHexDump(temp.data(), static_cast<size_t>(bytes));
            continue;
        }
        if (bytes == 0) {
            // Peer closed the connection.
            CloseClient(fd);
            return;
        }
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            break;
        }
        Log::Info(std::string("recv error: ") + std::strerror(errno));
        CloseClient(fd);
        return;
    }
}

void GameServer::CloseClient(int fd) {
    // Remove from epoll and erase from the connection map.
    epoll_.remove(fd);
    clients_.erase(fd);
}

void GameServer::LogHexDump(const uint8_t* data, size_t size) const {
    // Convert raw bytes to a printable hex string.
    std::ostringstream oss;
    oss << "GameServer RX (" << size << " bytes): ";
    for (size_t i = 0; i < size; ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<int>(data[i]);
        if (i + 1 < size) {
            oss << ' ';
        }
    }
    Log::Info(oss.str());
}
