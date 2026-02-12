/*
 * Copyright (c) DarkEmu
 * ConnectServer process entry point.
 */

#include "ConnectServer/ServerEngine.h"

#include <exception>
#include <iostream>
#include <memory>

int main() {
    try {
        // Instantiate and run the ConnectServer until it is terminated.
        auto server = std::make_unique<ServerEngine>();
        server->run();
    } catch (const std::exception& ex) {
        // Report startup/runtime failures to stderr for debugging.
        std::cerr << "ConnectServer failed: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}
