# DIY Software Renderer

A from-scratch 3D software renderer built to learn graphics programming fundamentals.

## Philosophy

This project uses **minimal external dependencies** - only SDL3 for windowing and pixel display. Everything else (math, rasterization, transformations, lighting, textures) is implemented from scratch.

## Quick Start

### Prerequisites

**Linux (Ubuntu/Debian)**
```bash
sudo apt-get install build-essential cmake libsdl3-dev
```

**macOS**
```bash
brew install cmake sdl3
```

**Windows**
- Install Visual Studio or MinGW
- Download SDL3 from https://www.libsdl.org/ or use vcpkg: `vcpkg install sdl3`

### Build and Run

```bash
mkdir build
cd build
cmake ..
make
./renderer
```

Press ESC to quit.

## Project Structure

```
diy-engine/
├── src/
│   ├── main.cpp              # Entry point with main loop
│   ├── core/                 # Framebuffer and window
│   │   ├── framebuffer.h/cpp # Pixel buffer (you render here!)
│   │   └── window.h/cpp      # SDL window wrapper
│   ├── math/                 # Vector/matrix math (implement yourself!)
│   └── rendering/            # Rasterizer, pipeline (implement yourself!)
├── assets/                   # Textures and models
├── ROADMAP.md               # Complete learning path
├── ACTION_PLAN.md           # Detailed implementation guide
└── GETTING_STARTED.md       # Quick start tutorial
```

## Learning Path

Check out these guides:
1. **ROADMAP.md** - 8-phase incremental learning plan
2. **ACTION_PLAN.md** - Detailed algorithms and implementation details
3. **GETTING_STARTED.md** - Tutorial with working code examples

## Current Status

**Phase 1: Foundation** ✓
- [x] Window creation
- [x] Framebuffer implementation
- [x] Pixel rendering
- [x] Example gradient display

**Phase 2: 2D Primitives** (Next up!)
- [ ] Line drawing (Bresenham's algorithm)
- [ ] Triangle outlines
- [ ] Filled shapes

## What You'll Implement

Everything beyond basic windowing:
- Vector and matrix math
- 3D transformations and projections
- Triangle rasterization
- Z-buffering
- Texture mapping
- Lighting calculations
- Normal mapping
- Shadow rendering

## Resources

- [Scratchapixel](https://www.scratchapixel.com/) - Excellent graphics tutorials
- [Tiny Renderer](https://github.com/ssloy/tinyrenderer) - Similar project
- [Learn OpenGL](https://learnopengl.com/) - Concepts apply to software rendering

## License

See LICENSE file for details.
