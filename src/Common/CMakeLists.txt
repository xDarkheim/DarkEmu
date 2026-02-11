# Copyright (c) DarkEmu
# Build rules for the shared common library.

add_library(DarkEmuCommon STATIC
    Network/Socket.cpp
    Network/EpollContext.cpp
    Utils/Logger.cpp
)

# Backward-compatible alias for older build scripts.
add_library(DarkheimCommon ALIAS DarkEmuCommon)

# Expose public headers for downstream targets.
target_include_directories(DarkEmuCommon PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# Ensure consumers compile with the expected C++ standard.
target_compile_features(DarkEmuCommon PUBLIC cxx_std_20)
