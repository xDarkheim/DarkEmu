/*
 * Copyright (c) DarkEmu
 * Logger implementation placeholder.
 */

#include "Common/Utils/Logger.h"

#include <iostream>

void Log::Info(std::string_view message) {
    // Send informational logs to stdout for now.
    std::cout << message << '\n';
}
