/*
 * Copyright (c) DarkEmu
 * ConnectServer event loop and server list response handling.
 */

#include "ConnectServer/ServerEngine.h"

#include "ConnectServer/Managers/ServerListManager.h"

#include <algorithm>
#include <array>
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <span>
#include <sys/socket.h>

ServerEngine::ServerEngine(uint16_t port) : events_(64), port_(port) {
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

    // Load server list data at startup.
    ServerListManager::Instance()->Load();
}

void ServerEngine::run() {
    // Main event loop: wait for epoll events and dispatch them.
    std::cout << "ConnectServer listening on port " << port_ << '\n';
    while (true) {
        int ready = epoll_.wait(events_, -1);
        for (int i = 0; i < ready; ++i) {
            const epoll_event& ev = events_[i];
            int fd = ev.data.fd;
            if (fd == listen_socket_.fd()) {
                // Accept new connections from the listening socket.
                if (ev.events & EPOLLIN) {
                    handleAccept();
                }
                continue;
            }
            // Close clients on error or hang-up events.
            if (ev.events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
                closeClient(fd);
                continue;
            }
            // Read available data from connected clients.
            if (ev.events & EPOLLIN) {
                handleRead(fd);
            }
        }
    }
}

void ServerEngine::runOnce(int timeoutMs) {
    // Single iteration of the event loop (useful for tests).
    int ready = epoll_.wait(events_, timeoutMs);
    for (int i = 0; i < ready; ++i) {
        const epoll_event& ev = events_[i];
        int fd = ev.data.fd;
        if (fd == listen_socket_.fd()) {
            // Accept new connections from the listening socket.
            if (ev.events & EPOLLIN) {
                handleAccept();
            }
            continue;
        }
        // Close clients on error or hang-up events.
        if (ev.events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
            closeClient(fd);
            continue;
        }
        // Read available data from connected clients.
        if (ev.events & EPOLLIN) {
            handleRead(fd);
        }
    }
}

uint16_t ServerEngine::port() const noexcept {
    return port_;
}

void ServerEngine::handleAccept() {
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

void ServerEngine::handleClientEvent(int fd, uint32_t events) {
    // Handle error/hang-up first, then read events.
    if (events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
        closeClient(fd);
        return;
    }
    if (events & EPOLLIN) {
        handleRead(fd);
    }
}

void ServerEngine::handleRead(int fd) {
    auto it = clients_.find(fd);
    if (it == clients_.end()) {
        return;
    }

    ClientState& client = it->second;
    std::array<uint8_t, 512> temp{};
    // Read until the socket would block or closes.
    while (true) {
        ssize_t bytes = client.socket.recv(temp);
        if (bytes > 0) {
            client.buffer.insert(client.buffer.end(), temp.begin(), temp.begin() + static_cast<size_t>(bytes));
            continue;
        }
        if (bytes == 0) {
            // Peer closed the connection.
            closeClient(fd);
            return;
        }
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            break;
        }
        std::cerr << "recv error: " << std::strerror(errno) << '\n';
        closeClient(fd);
        return;
    }

    // Wait for at least a minimal header before parsing.
    if (client.buffer.size() < 3) {
        return;
    }

    // Check for the server list request packet.
    if (client.buffer[0] == 0xC1 && client.buffer[2] == 0xF4) {
        static constexpr std::array<uint8_t, 4> kRequest{0xC1, 0x04, 0xF4, 0x06};
        if (client.buffer.size() >= kRequest.size()
            && std::equal(kRequest.begin(), kRequest.end(), client.buffer.begin())) {
            std::cout << "Server List Requested!" << '\n';
            sendServerListResponse(client.socket);
        }
    }
    // Close the client after responding (ConnectServer behavior).
    closeClient(fd);
}

void ServerEngine::closeClient(int fd) {
    // Remove from epoll and erase from the connection map.
    epoll_.remove(fd);
    clients_.erase(fd);
}

void ServerEngine::sendServerListResponse(Socket& client) {
    // Build a dynamic server list response packet.
    std::vector<uint8_t> response;
    ServerListManager::Instance()->GetPacket(response);

    std::span<const uint8_t> data(response);
    size_t offset = 0;
    // Send all bytes, handling partial sends on non-blocking sockets.
    while (offset < data.size()) {
        ssize_t sent = client.send(data.subspan(offset));
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
