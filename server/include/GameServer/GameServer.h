/**
 * Copyright (c) DarkEmu
 * GameServer networking entry points and state.
 */

#ifndef DARKEMU_GAMESERVER_H
#define DARKEMU_GAMESERVER_H

#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <vector>

#include "Common/Network/EpollContext.h"
#include "Common/Network/Socket.h"

/**
 * TCP game server that accepts clients and logs incoming packets.
 */
class GameServer {
public:
    /// Create the GameServer bound to the given port.
    explicit GameServer(uint16_t port = 55901);
    /// Run the main event loop indefinitely.
    void Run();
    /// Run a single event loop iteration (useful for tests).
    void RunOnce(int timeoutMs);
    /// Return the bound port for diagnostics or tests.
    uint16_t Port() const noexcept;
    /// Return total bytes received since startup (for testing).
    size_t BytesReceived() const noexcept;

private:
    /// Tracks per-client state for buffered reads.
    struct ClientState {
        Socket socket;                ///< Owned client socket.
        std::vector<uint8_t> buffer;  ///< Accumulated inbound data.
    };

    /// Accept all pending connections from the listen socket.
    void HandleAccept();
    /// Read from a connected client and log the payload.
    void HandleRead(int fd);
    /// Remove a client from the epoll set and internal map.
    void CloseClient(int fd);
    /// Convert raw bytes to a hex string and log it.
    void LogHexDump(const uint8_t* data, size_t size) const;

    EpollContext epoll_;
    Socket listen_socket_;
    std::unordered_map<int, ClientState> clients_;
    std::vector<epoll_event> events_;
    uint16_t port_{0};
    size_t total_bytes_received_{0};
};

#endif // DARKEMU_GAMESERVER_H
