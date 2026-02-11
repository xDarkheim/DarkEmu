#ifndef DARKHEIMMU_SOCKET_H
#define DARKHEIMMU_SOCKET_H

#include <cstdint>
#include <span>
#include <sys/socket.h>
#include <sys/types.h>

class Socket {
public:
    Socket() noexcept = default;
    explicit Socket(int fd) noexcept;
    ~Socket();

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    Socket(Socket&& other) noexcept;
    Socket& operator=(Socket&& other) noexcept;

    static Socket createTcp();

    bool isValid() const noexcept;
    int fd() const noexcept;

    void close() noexcept;

    void bind(uint16_t port, uint32_t address = 0);
    void listen(int backlog = SOMAXCONN);
    void setNonBlocking(bool enable);

    Socket accept();

    ssize_t recv(std::span<uint8_t> buffer, int flags = 0);
    ssize_t send(std::span<const uint8_t> buffer, int flags = 0);

private:
    int fd_{-1};
};

#endif // DARKHEIMMU_SOCKET_H
