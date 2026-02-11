# Copyright (c) DarkEmu
# Build rules for server tests.
find_package(Threads REQUIRED)

add_executable(CS_ProtocolTest
    cpp/ConnectServerProtocolTest.cpp
)

# Protocol test validates the server list packet response.
target_link_libraries(CS_ProtocolTest PRIVATE DarkheimCS_Lib DarkheimCommon Threads::Threads)

add_test(NAME CS_ProtocolTest COMMAND CS_ProtocolTest)

add_executable(CS_StressTest
    cpp/ConnectServerStressTest.cpp
)

# Stress test runs multiple clients in parallel.
target_link_libraries(CS_StressTest PRIVATE DarkheimCS_Lib DarkheimCommon Threads::Threads)

add_test(NAME CS_StressTest COMMAND CS_StressTest)

add_executable(GS_ConnectivityTest
    cpp/GameServerConnectivityTest.cpp
)

# Basic GameServer connectivity test.
target_link_libraries(GS_ConnectivityTest PRIVATE DarkheimGS_Lib DarkheimCommon Threads::Threads)

add_test(NAME GS_ConnectivityTest COMMAND GS_ConnectivityTest)
