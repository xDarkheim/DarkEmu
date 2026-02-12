# Building DarkEmu Client on Linux

The original client source (`client/mu-main-5.2/`) is a Windows Visual Studio project and cannot be directly compiled on Linux.

## Options

### Option 1: Use Wine (Recommended for Testing)
Run the compiled Windows client using Wine:

```bash
# Install Wine
sudo apt-get install wine wine32 wine64

# Copy Windows libraries
cp client/mu-main-5.2/bin/glew32.dll ~/.wine/drive_c/windows/system32/
cp client/mu-main-5.2/bin/ogg.dll ~/.wine/drive_c/windows/system32/
cp client/mu-main-5.2/bin/vorbisfile.dll ~/.wine/drive_c/windows/system32/

# Run the client
wine client/mu-main-5.2/Global\ Release/Main.exe
```

### Option 2: Cross-Compile for Windows
Use MinGW to cross-compile the Windows client on Linux:

```bash
# Install MinGW
sudo apt-get install mingw-w64

# Create build directory
mkdir client-build
cd client-build

# Configure with MinGW
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/mingw-toolchain.cmake ..

# Build
cmake --build . --parallel
```

### Option 3: Linux Native Client (Future)
Rewrite the client using cross-platform libraries:
- Replace DirectX with OpenGL
- Use GLFW for windowing
- Use Vorbis for audio
- Use CMake for building

A CMakeLists.txt template is provided in `client/CMakeLists.txt` for reference.

## Current Setup

The client source is Windows-only:
- Compiled with: DirectX, Windows API
- Platform: x86/x64 Windows only

For testing the server without a GUI client, use the included test suite:

```bash
ctest --test-dir cmake-build-debug --output-on-failure
```

