/*
 * Copyright (c) DarkEmu
 * epoll implementation for event-driven socket handling.
 */

#include "Common/Network/EpollContext.h"

#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <unistd.h>
#include <utility>

EpollContext::EpollContext() {
    // Create an epoll instance with close-on-exec flag.
    fd_ = ::epoll_create1(EPOLL_CLOEXEC);
    if (fd_ == -1) {
        throw std::runtime_error(std::strerror(errno));
    }
}

EpollContext::~EpollContext() {
    close();
}

EpollContext::EpollContext(EpollContext&& other) noexcept : fd_(std::exchange(other.fd_, -1)) {}

EpollContext& EpollContext::operator=(EpollContext&& other) noexcept {
    if (this != &other) {
        close();
        fd_ = std::exchange(other.fd_, -1);
    }
    return *this;
}

bool EpollContext::isValid() const noexcept {
    return fd_ >= 0;
}

int EpollContext::fd() const noexcept {
    return fd_;
}

void EpollContext::close() noexcept {
    // Close the descriptor if it is still open.
    if (fd_ >= 0) {
        ::close(fd_);
        fd_ = -1;
    }
}

void EpollContext::add(int fd, uint32_t events) {
    // Register the descriptor with the requested event mask.
    epoll_event ev{};
    ev.events = events;
    ev.data.fd = fd;
    if (::epoll_ctl(fd_, EPOLL_CTL_ADD, fd, &ev) == -1) {
        throw std::runtime_error(std::strerror(errno));
    }
}

void EpollContext::modify(int fd, uint32_t events) {
    // Update the descriptor's event mask.
    epoll_event ev{};
    ev.events = events;
    ev.data.fd = fd;
    if (::epoll_ctl(fd_, EPOLL_CTL_MOD, fd, &ev) == -1) {
        throw std::runtime_error(std::strerror(errno));
    }
}

void EpollContext::remove(int fd) {
    // Remove the descriptor from the epoll set.
    if (::epoll_ctl(fd_, EPOLL_CTL_DEL, fd, nullptr) == -1) {
        throw std::runtime_error(std::strerror(errno));
    }
}

int EpollContext::wait(std::span<epoll_event> events, int timeoutMs) {
    // Wait for events and translate EINTR to an empty result.
    int count = ::epoll_wait(fd_, events.data(), static_cast<int>(events.size()), timeoutMs);
    if (count == -1) {
        if (errno == EINTR) {
            return 0;
        }
        throw std::runtime_error(std::strerror(errno));
    }
    return count;
}
