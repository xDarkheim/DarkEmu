#ifndef DARKEMU_EPOLLCONTEXT_H
#define DARKEMU_EPOLLCONTEXT_H

#include <span>
#include <sys/epoll.h>

class EpollContext {
public:
    EpollContext();
    ~EpollContext();

    EpollContext(const EpollContext&) = delete;
    EpollContext& operator=(const EpollContext&) = delete;

    EpollContext(EpollContext&& other) noexcept;
    EpollContext& operator=(EpollContext&& other) noexcept;

    bool isValid() const noexcept;
    int fd() const noexcept;

    void close() noexcept;

    void add(int fd, uint32_t events);
    void modify(int fd, uint32_t events);
    void remove(int fd);

    int wait(std::span<epoll_event> events, int timeoutMs);

private:
    int fd_{-1};
};

#endif // DARKEMU_EPOLLCONTEXT_H
