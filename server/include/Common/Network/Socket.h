/*
 * Copyright (c) DarkEmu
 * RAII wrapper interface for TCP sockets.
 */

#ifndef DARKEMU_SOCKET_H
#define DARKEMU_SOCKET_H

#include <cstdint>
#include <span>
#include <sys/socket.h>
#include <sys/types.h>

/**
 * Lightweight RAII wrapper for a TCP socket file descriptor.
 * Owns the descriptor, closes it on destruction, and provides basic operations.
 */

class Socket {
public:
    /// Create an invalid socket wrapper.
    Socket() noexcept = default;
    /// Adopt an existing file descriptor (assumes ownership).
    explicit Socket(int fd) noexcept;
    /// Close the socket if it is still open.
    ~Socket();

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    /// Move ownership from another Socket.
    Socket(Socket&& other) noexcept;
    /// Move-assign ownership and close the existing descriptor.
    Socket& operator=(Socket&& other) noexcept;

    /// Create a new TCP socket or throw on failure.
    static Socket createTcp();

    /// Check whether the socket currently owns a valid descriptor.
    bool isValid() const noexcept;
    /// Expose the underlying file descriptor.
    int fd() const noexcept;

    /// Close the socket and reset the descriptor to invalid.
    void close() noexcept;

    /**
     * Bind the socket to the given port and address.
     * @param port Local TCP port to bind.
     * @param address IPv4 address in host byte order (default: INADDR_ANY).
     */
    void bind(uint16_t port, uint32_t address = 0);
    /// Mark the socket as a listening socket.
    void listen(int backlog = SOMAXCONN);
    /// Toggle non-blocking mode on the descriptor.
    void setNonBlocking(bool enable);

    /// Accept an incoming connection; returns invalid Socket on EAGAIN.
    Socket accept();

    /// Receive data into the provided buffer.
    ssize_t recv(std::span<uint8_t> buffer, int flags = 0);
    /// Send data from the provided buffer.
    ssize_t send(std::span<const uint8_t> buffer, int flags = 0);

private:
    /// Owned file descriptor, -1 means invalid.
    int fd_{-1};
};

#endif // DARKEMU_SOCKET_H
