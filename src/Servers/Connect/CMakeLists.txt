# Copyright (c) DarkEmu
# Build rules for the ConnectServer module.

add_library(ConnectServerCore STATIC
    ServerEngine.cpp
    Managers/ServerListManager.cpp
    Packets/PacketHandler.cpp
)

# Link common networking utilities into the ConnectServer core.
target_link_libraries(ConnectServerCore PUBLIC DarkEmuCommon)

# Make public headers visible to consumers (tests, server binary).
target_include_directories(ConnectServerCore PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# Allow inclusion of bundled third-party headers under src/ (json.hpp).
target_include_directories(ConnectServerCore PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)

# ConnectServer executable entry point.
add_executable(ConnectServer
    main.cpp
)

# Link the server binary against the core library.
target_link_libraries(ConnectServer PRIVATE ConnectServerCore)
