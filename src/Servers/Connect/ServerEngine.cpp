#include "ConnectServer/ServerEngine.h"

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
    listen_socket_ = Socket::createTcp();
    listen_socket_.setNonBlocking(true);
    listen_socket_.bind(port_);
    listen_socket_.listen();
    sockaddr_in addr{};
    socklen_t addr_len = sizeof(addr);
    if (::getsockname(listen_socket_.fd(), reinterpret_cast<sockaddr*>(&addr), &addr_len) == -1) {
        throw std::runtime_error(std::strerror(errno));
    }
    port_ = ntohs(addr.sin_port);
    epoll_.add(listen_socket_.fd(), EPOLLIN);
}

void ServerEngine::run() {
    std::cout << "ConnectServer listening on port " << port_ << '\n';
    while (true) {
        int ready = epoll_.wait(events_, -1);
        for (int i = 0; i < ready; ++i) {
            const epoll_event& ev = events_[i];
            int fd = ev.data.fd;
            if (fd == listen_socket_.fd()) {
                if (ev.events & EPOLLIN) {
                    handleAccept();
                }
                continue;
            }
            if (ev.events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
                closeClient(fd);
                continue;
            }
            if (ev.events & EPOLLIN) {
                handleRead(fd);
            }
        }
    }
}

void ServerEngine::runOnce(int timeoutMs) {
    int ready = epoll_.wait(events_, timeoutMs);
    for (int i = 0; i < ready; ++i) {
        const epoll_event& ev = events_[i];
        int fd = ev.data.fd;
        if (fd == listen_socket_.fd()) {
            if (ev.events & EPOLLIN) {
                handleAccept();
            }
            continue;
        }
        if (ev.events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
            closeClient(fd);
            continue;
        }
        if (ev.events & EPOLLIN) {
            handleRead(fd);
        }
    }
}

uint16_t ServerEngine::port() const noexcept {
    return port_;
}

void ServerEngine::handleAccept() {
    while (true) {
        Socket client = listen_socket_.accept();
        if (!client.isValid()) {
            break;
        }
        client.setNonBlocking(true);
        int fd = client.fd();
        epoll_.add(fd, EPOLLIN | EPOLLRDHUP);
        clients_.emplace(fd, ClientState{std::move(client), {}});
    }
}

void ServerEngine::handleClientEvent(int fd, uint32_t events) {
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
    while (true) {
        ssize_t bytes = client.socket.recv(temp);
        if (bytes > 0) {
            client.buffer.insert(client.buffer.end(), temp.begin(), temp.begin() + static_cast<size_t>(bytes));
            continue;
        }
        if (bytes == 0) {
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

    if (client.buffer.size() < 3) {
        return;
    }

    if (client.buffer[0] == 0xC1 && client.buffer[2] == 0xF4) {
        static constexpr std::array<uint8_t, 4> kRequest{0xC1, 0x04, 0xF4, 0x06};
        if (client.buffer.size() >= kRequest.size()
            && std::equal(kRequest.begin(), kRequest.end(), client.buffer.begin())) {
            std::cout << "Server List Requested!" << '\n';
            sendServerListResponse(client.socket);
        }
    }
    closeClient(fd);
}

void ServerEngine::closeClient(int fd) {
    epoll_.remove(fd);
    clients_.erase(fd);
}

void ServerEngine::sendServerListResponse(Socket& client) {
    std::array<uint8_t, 8> response = {0xC2, 0x00, 0xF4, 0x06, 0x01, 0x00, 0x01, 0x00};
    response[1] = static_cast<uint8_t>(response.size());

    std::span<const uint8_t> data(response);
    size_t offset = 0;
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
