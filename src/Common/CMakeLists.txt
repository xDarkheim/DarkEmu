add_library(DarkheimCommon STATIC
    Network/Socket.cpp
    Network/EpollContext.cpp
    Utils/Logger.cpp
)

target_include_directories(DarkheimCommon PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(DarkheimCommon PUBLIC cxx_std_20)
