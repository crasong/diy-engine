# Software Renderer Roadmap

A step-by-step guide to building a software renderer from scratch.

## Table of Contents
1. [Foundation](#phase-1-foundation)
2. [2D Primitives](#phase-2-2d-primitives)
3. [3D Basics](#phase-3-3d-basics)
4. [Triangle Rasterization](#phase-4-triangle-rasterization)
5. [Depth & Perspective](#phase-5-depth--perspective)
6. [Texturing](#phase-6-texturing)
7. [Lighting](#phase-7-lighting)
8. [Advanced Features](#phase-8-advanced-features)

---

## Phase 1: Foundation
**Goal**: Set up the basic infrastructure to display pixels on screen

### What You'll Learn
- How pixels are stored in memory (framebuffer)
- Coordinate systems and screen space
- Basic color representation (RGB/RGBA)

### Implementation Tasks
- [ ] Create a window (using SDL2, GLFW, or platform-specific APIs)
- [ ] Allocate a framebuffer (array of pixels in memory)
- [ ] Implement `setPixel(x, y, color)` function
- [ ] Display the framebuffer to the window
- [ ] Add basic color manipulation (RGB to hex, color blending)

### Milestone
Draw a simple pattern (checkerboard, gradient, or noise) to verify pixel rendering works.

---

## Phase 2: 2D Primitives
**Goal**: Implement basic 2D drawing functions

### What You'll Learn
- Line drawing algorithms (Bresenham's algorithm)
- 2D coordinate transformations
- Clipping to screen bounds

### Implementation Tasks
- [ ] Implement line drawing (Bresenham's algorithm)
- [ ] Draw wireframe rectangles and circles
- [ ] Implement filled rectangles
- [ ] Add circle rasterization (midpoint circle algorithm)
- [ ] Implement basic 2D transformations (translate, rotate, scale)

### Milestone
Draw a simple 2D scene with multiple shapes (e.g., a house with windows and a sun).

---

## Phase 3: 3D Basics
**Goal**: Understand 3D space and project it to 2D

### What You'll Learn
- 3D coordinate systems (world space, view space, screen space)
- Homogeneous coordinates and 4x4 matrices
- Perspective and orthographic projection
- Camera transformations (view matrix)

### Implementation Tasks
- [ ] Implement vector math (Vec3, Vec4 classes with dot/cross products)
- [ ] Implement matrix math (Mat4x4 class with multiplication)
- [ ] Create projection matrices (perspective and orthographic)
- [ ] Implement view matrix (look-at camera)
- [ ] Project 3D points to 2D screen coordinates
- [ ] Draw a wireframe cube rotating in 3D

### Milestone
Render a spinning wireframe cube with proper perspective projection.

---

## Phase 4: Triangle Rasterization
**Goal**: Fill triangles with solid colors

### What You'll Learn
- Barycentric coordinates
- Scanline algorithm or edge function approach
- Top-left fill rule (to avoid gaps/overlaps)

### Implementation Tasks
- [ ] Implement triangle rasterization (edge function or scanline)
- [ ] Handle degenerate triangles (zero area)
- [ ] Implement flat-top/flat-bottom triangle optimization (optional)
- [ ] Add wireframe vs filled rendering modes
- [ ] Render a 3D mesh as solid colored triangles

### Milestone
Render a 3D cube with filled, colored faces (each face a different color).

---

## Phase 5: Depth & Perspective
**Goal**: Handle depth correctly and add perspective-correct interpolation

### What You'll Learn
- Z-buffering (depth testing)
- Perspective-correct interpolation
- Hidden surface removal

### Implementation Tasks
- [ ] Implement a Z-buffer (depth buffer)
- [ ] Add depth testing during rasterization
- [ ] Implement perspective-correct attribute interpolation
- [ ] Handle near/far clipping planes
- [ ] Render overlapping 3D objects correctly

### Milestone
Render multiple overlapping 3D objects (cubes, pyramids) with correct depth ordering.

---

## Phase 6: Texturing
**Goal**: Map images onto 3D surfaces

### What You'll Learn
- UV coordinates
- Texture sampling and filtering
- Mipmapping (optional)
- Perspective-correct texture mapping

### Implementation Tasks
- [ ] Load image files (BMP, PNG, or TGA format)
- [ ] Implement texture class with sampling
- [ ] Add UV coordinates to vertices
- [ ] Implement perspective-correct texture interpolation
- [ ] Add texture filtering (nearest neighbor, then bilinear)
- [ ] Handle texture wrapping modes (repeat, clamp)

### Milestone
Render a textured 3D cube or a simple 3D model with a texture.

---

## Phase 7: Lighting
**Goal**: Add realistic lighting to your scenes

### What You'll Learn
- Normal vectors and their importance
- Diffuse lighting (Lambertian)
- Specular lighting (Phong/Blinn-Phong)
- Ambient lighting
- Multiple light sources

### Implementation Tasks
- [ ] Calculate/store normal vectors for each triangle
- [ ] Implement ambient lighting (constant)
- [ ] Implement diffuse lighting (Lambertian reflection)
- [ ] Add specular highlights (Phong or Blinn-Phong)
- [ ] Support multiple point lights
- [ ] Implement Gouraud shading (vertex lighting)
- [ ] Implement Phong shading (per-pixel lighting)

### Milestone
Render a textured, lit sphere or model that responds to moving light sources.

---

## Phase 8: Advanced Features
**Goal**: Add polish and advanced rendering techniques

### What You'll Learn
- Normal mapping
- Shadow mapping
- Alpha blending/transparency
- Backface culling
- Frustum culling
- Anti-aliasing techniques

### Implementation Tasks

#### Optimization
- [ ] Implement backface culling
- [ ] Add frustum culling
- [ ] Implement spatial partitioning (optional: octree, BSP)
- [ ] Multi-threading for rasterization

#### Visual Quality
- [ ] Implement normal mapping
- [ ] Add alpha blending for transparency
- [ ] Implement shadow mapping or shadow volumes
- [ ] Add MSAA or FXAA anti-aliasing
- [ ] Implement skybox rendering

#### Advanced Lighting
- [ ] Add directional and spot lights
- [ ] Implement attenuation for point lights
- [ ] Add basic fog effects

### Milestone
Render a complete scene with multiple textured, lit objects, shadows, and transparency.

---

## Recommended Resources

### Math Foundations
- Linear algebra: vectors, matrices, transformations
- Trigonometry: sine, cosine, angles
- 3D geometry: cross product, dot product, plane equations

### Algorithms
- Bresenham's line algorithm
- Barycentric coordinates
- Triangle rasterization techniques
- Z-buffering

### Books & References
- "Computer Graphics: Principles and Practice" (Foley et al.)
- "Fundamentals of Computer Graphics" (Shirley & Marschner)
- scratchapixel.com - excellent tutorials
- learnopengl.com - concepts apply to software rendering too

### Model Formats
- Start with OBJ format (simple, text-based, widely supported)
- Parse vertex positions, normals, and UV coordinates

---

## Development Tips

### Start Simple
- Don't try to implement everything at once
- Get each phase working before moving to the next
- Test with simple shapes before complex models

### Debugging
- Implement wireframe mode to debug rasterization
- Color-code different aspects (normals, UVs, depth)
- Save framebuffer to image files for comparison
- Use unit tests for math operations

### Performance
- Don't optimize prematurely
- Profile before optimizing
- Integer math can be faster than floating-point for some operations
- SIMD instructions can help (SSE, AVX)

### Code Organization
Suggested structure:
```
src/
  ├── math/           # Vector, matrix, quaternion math
  ├── core/           # Framebuffer, color, window
  ├── geometry/       # Mesh, vertex, triangle structures
  ├── rendering/      # Rasterizer, pipeline, shaders
  ├── textures/       # Texture loading and sampling
  └── scene/          # Camera, lights, scene graph
```

---

## Testing Each Phase

### Validation Checklist
- ✓ Does it produce expected output for simple cases?
- ✓ Does it handle edge cases (zero-size, out-of-bounds)?
- ✓ Is the math numerically stable?
- ✓ Can you visualize intermediate steps?

### Test Cases
- Render reference models (Utah teapot, Stanford bunny)
- Compare output against OpenGL/DirectX renders
- Test with extreme values (very close, very far objects)

---

## Final Project Ideas

Once you've completed the phases, try these:
1. **Model Viewer** - Load and display .obj files with lighting
2. **Simple Game** - Create a 3D maze or simple FPS
3. **Ray Tracer** - Switch from rasterization to ray tracing
4. **Demo Scene** - Create an animated 3D scene
5. **Software GPU** - Implement a programmable shader pipeline

---

Good luck on your journey! Remember: every modern GPU does these steps in hardware. By doing it in software, you'll deeply understand how 3D graphics really work.
