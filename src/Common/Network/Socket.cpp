#include "Common/Network/Socket.h"

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdexcept>
#include <unistd.h>
#include <utility>

Socket::Socket(int fd) noexcept : fd_(fd) {}

Socket::~Socket() {
    close();
}

Socket::Socket(Socket&& other) noexcept : fd_(std::exchange(other.fd_, -1)) {}

Socket& Socket::operator=(Socket&& other) noexcept {
    if (this != &other) {
        close();
        fd_ = std::exchange(other.fd_, -1);
    }
    return *this;
}

Socket Socket::createTcp() {
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        throw std::runtime_error(std::strerror(errno));
    }
    return Socket(fd);
}

bool Socket::isValid() const noexcept {
    return fd_ >= 0;
}

int Socket::fd() const noexcept {
    return fd_;
}

void Socket::close() noexcept {
    if (fd_ >= 0) {
        ::close(fd_);
        fd_ = -1;
    }
}

void Socket::bind(uint16_t port, uint32_t address) {
    if (!isValid()) {
        throw std::runtime_error("bind on invalid socket");
    }

    int reuse = 1;
    if (::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
        throw std::runtime_error(std::strerror(errno));
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(address);

    if (::bind(fd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) {
        throw std::runtime_error(std::strerror(errno));
    }
}

void Socket::listen(int backlog) {
    if (::listen(fd_, backlog) == -1) {
        throw std::runtime_error(std::strerror(errno));
    }
}

void Socket::setNonBlocking(bool enable) {
    int flags = ::fcntl(fd_, F_GETFL, 0);
    if (flags == -1) {
        throw std::runtime_error(std::strerror(errno));
    }
    if (enable) {
        flags |= O_NONBLOCK;
    } else {
        flags &= ~O_NONBLOCK;
    }
    if (::fcntl(fd_, F_SETFL, flags) == -1) {
        throw std::runtime_error(std::strerror(errno));
    }
}

Socket Socket::accept() {
    sockaddr_in addr{};
    socklen_t len = sizeof(addr);
    int client_fd = ::accept(fd_, reinterpret_cast<sockaddr*>(&addr), &len);
    if (client_fd == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return Socket();
        }
        throw std::runtime_error(std::strerror(errno));
    }
    return Socket(client_fd);
}

ssize_t Socket::recv(std::span<uint8_t> buffer, int flags) {
    return ::recv(fd_, buffer.data(), buffer.size(), flags);
}

ssize_t Socket::send(std::span<const uint8_t> buffer, int flags) {
    return ::send(fd_, buffer.data(), buffer.size(), flags);
}
