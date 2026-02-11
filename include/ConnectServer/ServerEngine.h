#ifndef DARKEMU_SERVERENGINE_H
#define DARKEMU_SERVERENGINE_H

#include <cstdint>
#include <unordered_map>
#include <vector>

#include "Common/Network/EpollContext.h"
#include "Common/Network/Socket.h"

class ServerEngine {
public:
    explicit ServerEngine(uint16_t port = 44405);
    void run();
    void runOnce(int timeoutMs);
    uint16_t port() const noexcept;

private:
    struct ClientState {
        Socket socket;
        std::vector<uint8_t> buffer;
    };

    void handleAccept();
    void handleClientEvent(int fd, uint32_t events);
    void handleRead(int fd);
    void closeClient(int fd);
    void sendServerListResponse(Socket& client);

    EpollContext epoll_;
    Socket listen_socket_;
    std::unordered_map<int, ClientState> clients_;
    std::vector<epoll_event> events_;
    uint16_t port_{0};
};

#endif // DARKEMU_SERVERENGINE_H
