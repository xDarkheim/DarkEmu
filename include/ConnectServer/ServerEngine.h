/*
 * Copyright (c) DarkEmu
 * ConnectServer event loop and client handling interface.
 */

#ifndef DARKEMU_SERVERENGINE_H
#define DARKEMU_SERVERENGINE_H

#include <cstdint>
#include <unordered_map>
#include <vector>

#include "Common/Network/EpollContext.h"
#include "Common/Network/Socket.h"

/**
 * ConnectServer engine that accepts clients and responds to server list requests.
 * Uses epoll for scalable event handling.
 */

class ServerEngine {
public:
    /// Create a server engine bound to the given port (0 selects an ephemeral port).
    explicit ServerEngine(uint16_t port = 44405);
    /// Run the server event loop indefinitely.
    void run();
    /// Run a single epoll wait/dispatch cycle (used by tests).
    void runOnce(int timeoutMs);
    /// Return the actual port bound by the listening socket.
    uint16_t port() const noexcept;

private:
    /// Tracks per-client state for buffered reads.
    struct ClientState {
        Socket socket;                ///< Owned client socket.
        std::vector<uint8_t> buffer;  ///< Accumulated inbound data.
    };

    /// Accept all pending connections from the listen socket.
    void handleAccept();
    /// Dispatch events for a client descriptor.
    void handleClientEvent(int fd, uint32_t events);
    /// Read and process data for a connected client.
    void handleRead(int fd);
    /// Remove a client from the epoll set and internal map.
    void closeClient(int fd);

    EpollContext epoll_;
    Socket listen_socket_;
    std::unordered_map<int, ClientState> clients_;
    std::vector<epoll_event> events_;
    uint16_t port_{0};
};

#endif // DARKEMU_SERVERENGINE_H
