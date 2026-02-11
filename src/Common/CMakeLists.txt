add_library(DarkEmuCommon STATIC
    Network/Socket.cpp
    Network/EpollContext.cpp
    Utils/Logger.cpp
)

target_include_directories(DarkEmuCommon PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(DarkEmuCommon PUBLIC cxx_std_20)
