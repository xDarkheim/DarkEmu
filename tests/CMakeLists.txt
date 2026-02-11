add_executable(ConnectServerProtocolTest
    ConnectServerProtocolTest.cpp
)

target_link_libraries(ConnectServerProtocolTest PRIVATE ConnectServerCore)

add_test(NAME ConnectServerProtocolTest COMMAND ConnectServerProtocolTest)

add_executable(ConnectServerStressTest
    ConnectServerStressTest.cpp
)

target_link_libraries(ConnectServerStressTest PRIVATE ConnectServerCore)

add_test(NAME ConnectServerStressTest COMMAND ConnectServerStressTest)
