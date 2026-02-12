/*
 * Copyright (c) DarkEmu
 * RAII wrapper interface for a Linux epoll instance.
 */

#ifndef DARKEMU_EPOLLCONTEXT_H
#define DARKEMU_EPOLLCONTEXT_H

#include <span>
#include <sys/epoll.h>

/**
 * RAII wrapper for an epoll instance.
 * Manages the epoll file descriptor and provides basic operations.
 */

class EpollContext {
public:
    /// Create a new epoll instance.
    EpollContext();
    /// Close the epoll file descriptor.
    ~EpollContext();

    EpollContext(const EpollContext&) = delete;
    EpollContext& operator=(const EpollContext&) = delete;

    /// Move ownership from another EpollContext.
    EpollContext(EpollContext&& other) noexcept;
    /// Move-assign ownership and close the existing descriptor.
    EpollContext& operator=(EpollContext&& other) noexcept;

    /// Check whether the epoll descriptor is valid.
    bool isValid() const noexcept;
    /// Expose the underlying epoll file descriptor.
    int fd() const noexcept;

    /// Close the epoll descriptor and reset to invalid.
    void close() noexcept;

    /**
     * Register a file descriptor for epoll events.
     * @param fd File descriptor to monitor.
     * @param events Epoll event mask.
     */
    void add(int fd, uint32_t events);
    /// Modify the event mask for a registered descriptor.
    void modify(int fd, uint32_t events);
    /// Remove a descriptor from epoll monitoring.
    void remove(int fd);

    /**
     * Wait for events and return the number ready.
     * @param events Destination buffer for events.
     * @param timeoutMs Timeout in milliseconds (-1 blocks).
     */
    int wait(std::span<epoll_event> events, int timeoutMs);

private:
    /// Owned epoll file descriptor, -1 means invalid.
    int fd_{-1};
};

#endif // DARKEMU_EPOLLCONTEXT_H
