# Copyright (c) DarkEmu
# Build rules for server tests.
find_package(Threads REQUIRED)

# Add common include directory for all tests
set(TEST_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/server/include)

add_executable(CS_ProtocolTest
    cpp/ConnectServerProtocolTest.cpp
)

# Protocol test validates the server list packet response.
target_link_libraries(CS_ProtocolTest PRIVATE DarkheimCS_Lib DarkheimCommon Threads::Threads)
target_include_directories(CS_ProtocolTest PRIVATE ${TEST_INCLUDE_DIRS})

add_test(NAME CS_ProtocolTest COMMAND CS_ProtocolTest)

add_executable(CS_StressTest
    cpp/ConnectServerStressTest.cpp
)

# Stress test runs multiple clients in parallel.
target_link_libraries(CS_StressTest PRIVATE DarkheimCS_Lib DarkheimCommon Threads::Threads)
target_include_directories(CS_StressTest PRIVATE ${TEST_INCLUDE_DIRS})

add_test(NAME CS_StressTest COMMAND CS_StressTest)

add_executable(GS_ConnectivityTest
    cpp/GameServerConnectivityTest.cpp
)

# Basic GameServer connectivity test.
target_link_libraries(GS_ConnectivityTest PRIVATE DarkheimGS_Lib DarkheimCommon Threads::Threads)
target_include_directories(GS_ConnectivityTest PRIVATE ${TEST_INCLUDE_DIRS})

add_test(NAME GS_ConnectivityTest COMMAND GS_ConnectivityTest)
