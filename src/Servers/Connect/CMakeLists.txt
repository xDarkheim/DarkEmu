# Copyright (c) DarkEmu
# Build rules for the ConnectServer module.

add_library(DarkheimCS_Lib STATIC
    ServerEngine.cpp
    Managers/ServerListManager.cpp
    Packets/PacketHandler.cpp
    ${PROJECT_SOURCE_DIR}/include/ConnectServer/ServerEngine.h
    ${PROJECT_SOURCE_DIR}/include/ConnectServer/Managers/ServerListManager.h
    ${PROJECT_SOURCE_DIR}/include/ConnectServer/Packets/PacketHandler.h
)

# Link common networking utilities into the ConnectServer core.
target_link_libraries(DarkheimCS_Lib PUBLIC DarkheimCommon)

# Make public headers visible to consumers (tests, server binary).
target_include_directories(DarkheimCS_Lib PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# Allow inclusion of bundled third-party headers under src/ (json.hpp).
target_include_directories(DarkheimCS_Lib PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)

# ConnectServer executable entry point.
add_executable(DarkheimCS
    main.cpp
)

# Link the server binary against the core library.
target_link_libraries(DarkheimCS PRIVATE DarkheimCS_Lib)
