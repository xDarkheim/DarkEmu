# Copyright (c) DarkEmu
# Build rules for the GameServer module.

add_library(DarkheimGS_Lib STATIC
    GameServer.cpp
    ${PROJECT_SOURCE_DIR}/server/include/GameServer/GameServer.h
)

# Link against the shared common networking library.
target_link_libraries(DarkheimGS_Lib PUBLIC DarkheimCommon)

# Expose shared headers to the GameServer target.
target_include_directories(DarkheimGS_Lib PUBLIC
    ${PROJECT_SOURCE_DIR}/server/include
)

# GameServer executable entry point.
add_executable(DarkheimGS
    main.cpp
)

# Link the server binary against the core library.
target_link_libraries(DarkheimGS PRIVATE DarkheimGS_Lib)
