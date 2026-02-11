# Copyright (c) DarkEmu
# Build rules for ConnectServer tests.
add_executable(ConnectServerProtocolTest
    ConnectServerProtocolTest.cpp
)

# Protocol test validates the server list packet response.
target_link_libraries(ConnectServerProtocolTest PRIVATE ConnectServerCore)

add_test(NAME ConnectServerProtocolTest COMMAND ConnectServerProtocolTest)

add_executable(ConnectServerStressTest
    ConnectServerStressTest.cpp
)

# Stress test runs multiple clients in parallel.
target_link_libraries(ConnectServerStressTest PRIVATE ConnectServerCore)

add_test(NAME ConnectServerStressTest COMMAND ConnectServerStressTest)
