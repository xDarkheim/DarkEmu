#include "ConnectServer/ServerEngine.h"

#include <exception>
#include <iostream>
#include <memory>

int main() {
    try {
        auto server = std::make_unique<ServerEngine>();
        server->run();
    } catch (const std::exception& ex) {
        std::cerr << "ConnectServer failed: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}
