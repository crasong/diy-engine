# Build Instructions

This project uses CMake and can automatically download and build SDL3 from source, making it completely self-contained.

## Quick Start (Automatic SDL3)

The easiest way - CMake will download and build SDL3 for you:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

**First build takes longer** (~2-5 minutes) because it downloads and compiles SDL3. Subsequent builds are fast.

### Run the renderer

**Linux/macOS:**
```bash
./renderer
```

**Windows:**
```bash
.\renderer.exe
# or
.\Debug\renderer.exe  # if using Visual Studio
```

---

## Build Options

### Option 1: FetchContent (Default - Recommended)

Automatically downloads SDL3 from GitHub and builds it:

```bash
cmake ..
# OR explicitly:
cmake -DUSE_SYSTEM_SDL3=OFF ..
```

**Pros:**
- Self-contained - works out of the box
- No manual SDL3 installation needed
- Version-controlled (uses specific SDL3 version)
- Cross-platform

**Cons:**
- First build is slower
- Requires internet connection on first build

---

### Option 2: System SDL3

Use SDL3 installed on your system:

```bash
cmake -DUSE_SYSTEM_SDL3=ON ..
```

**Install SDL3 first:**

**Linux (Ubuntu/Debian):**
```bash
# SDL3 may not be in repos yet, build from source:
git clone https://github.com/libsdl-org/SDL.git
cd SDL
mkdir build && cd build
cmake ..
sudo make install
```

**macOS:**
```bash
brew install sdl3
# If not available yet:
brew install --HEAD sdl3
```

**Windows:**
- Download from https://github.com/libsdl-org/SDL/releases
- Or use vcpkg: `vcpkg install sdl3`

---

## Platform-Specific Instructions

### Windows (Visual Studio)

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release

# Run
.\Release\renderer.exe
```

Or open `DIYEngine.sln` in Visual Studio.

### Windows (MinGW)

```bash
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
cmake --build .
.\renderer.exe
```

### Linux

```bash
mkdir build
cd build
cmake ..
make -j$(nproc)  # Parallel build
./renderer
```

### macOS

```bash
mkdir build
cd build
cmake ..
make -j$(sysctl -n hw.ncpu)  # Parallel build
./renderer
```

---

## Troubleshooting

### "Could not find SDL3"

If using system SDL3 (`USE_SYSTEM_SDL3=ON`), make sure it's installed and CMake can find it:

```bash
# Linux: Check if SDL3 is installed
ldconfig -p | grep SDL3

# Set SDL3 path manually if needed
cmake -DSDL3_DIR=/path/to/SDL3/cmake ..
```

### FetchContent fails (network issues)

If the download fails:
1. Check your internet connection
2. Try using a specific SDL3 release instead of `main`:
   - Edit `CMakeLists.txt`, change `GIT_TAG main` to `GIT_TAG release-3.1.2`
3. Or clone SDL3 manually and use it as a system library (see Option 2)

### Build is very slow

First-time build with FetchContent is slower because it builds SDL3. To speed up:

```bash
# Use parallel builds
cmake --build . -j 8  # 8 parallel jobs

# Or with make:
make -j8
```

### Missing DLLs on Windows

After building, the SDL3.dll should be in the same directory as renderer.exe. If not:

```bash
# Copy SDL3.dll to executable directory
copy build\_deps\sdl3-build\Release\SDL3.dll build\Release\
```

Or add SDL3 to your PATH.

---

## Build Configurations

### Debug Build (default)
```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
```
- Slower execution
- Includes debug symbols
- Better for development

### Release Build
```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
```
- Optimized (-O3)
- Faster execution
- Use for performance testing

---

## Clean Build

If you need to start fresh:

```bash
# Delete build directory
rm -rf build

# Rebuild
mkdir build && cd build
cmake ..
cmake --build .
```

---

## CMake Cache

FetchContent downloads SDL3 to `build/_deps/`. To force re-download:

```bash
rm -rf build/_deps/sdl3-*
cmake ..
```

---

## Advanced: Using Your Precompiled SDL3.dll

If you already have SDL3.dll and want to use it:

**Option A: System install**
1. Put SDL3.dll, SDL3.lib, and headers in a standard location
2. Use `cmake -DUSE_SYSTEM_SDL3=ON ..`

**Option B: Manual paths**
```bash
cmake -DUSE_SYSTEM_SDL3=ON \
      -DSDL3_INCLUDE_DIR=/path/to/SDL3/include \
      -DSDL3_LIBRARY=/path/to/SDL3.lib \
      ..
```

**Option C: Copy to build directory** (quickest for testing)
1. Build with FetchContent (default)
2. Replace `build/_deps/sdl3-build/SDL3.dll` with your version
3. Rebuild: `cmake --build .`

---

## Recommended Workflow

**For development:**
```bash
# One-time setup
mkdir build && cd build
cmake ..

# Daily workflow
cmake --build .  # Rebuild after code changes
./renderer       # Test
```

**For sharing:**
- Just push your code - others can clone and build immediately
- No need to distribute SDL3 separately
- Cross-platform out of the box

---

## Next Steps

After building successfully:
1. Run `./renderer` - you should see a gradient
2. Press ESC to quit
3. Start implementing Phase 2 features!
4. See ROADMAP.md for what to build next
