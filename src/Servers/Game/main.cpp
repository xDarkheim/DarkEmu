/**
 * Copyright (c) DarkEmu
 * GameServer process entry point.
 */

#include "GameServer/GameServer.h"

#include "Common/Utils/Logger.h"

#include <exception>
#include <string>

int main() {
    try {
        // Instantiate and run the GameServer until it is terminated.
        GameServer server;
        server.Run();
    } catch (const std::exception& ex) {
        // Report startup/runtime failures to stdout for now.
        Log::Info(std::string("GameServer failed: ") + ex.what());
        return 1;
    }
    return 0;
}
