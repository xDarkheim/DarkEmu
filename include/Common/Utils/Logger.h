/*
 * Copyright (c) DarkEmu
 * Logger interface placeholder for shared logging utilities.
 */

#ifndef DARKEMU_LOGGER_H
#define DARKEMU_LOGGER_H


#include <string_view>

/// Minimal logger stub used to keep the build structure intact.
/// TODO: Replace with a real logging interface and implementation.
class Logger {};

/// Lightweight logging helpers for early-stage server development.
namespace Log {
    /// Write an informational message to stdout.
    void Info(std::string_view message);
}


#endif // DARKEMU_LOGGER_H
