add_library(ConnectServerCore STATIC
    ServerEngine.cpp
)

target_link_libraries(ConnectServerCore PUBLIC DarkEmuCommon)

target_include_directories(ConnectServerCore PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

add_executable(ConnectServer
    main.cpp
)

target_link_libraries(ConnectServer PRIVATE ConnectServerCore)
