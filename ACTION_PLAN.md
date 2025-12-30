# Software Renderer Action Plan

Detailed implementation guide with code concepts and algorithms.

## Quick Start Guide

### Choose Your Language
- **C/C++**: Maximum performance, close to hardware
- **Rust**: Memory safety + performance
- **C#/Java**: Easier development, still good performance
- **Python**: Rapid prototyping (use NumPy for speed)

### Required External Dependencies (Minimal)
- **Window/Display**: SDL2, GLFW, or native APIs
- **Image Loading** (later): stb_image.h (single header library)
- **Optional**: Build system (CMake, Make, Cargo)

---

## Phase 1 Deep Dive: Foundation

### Framebuffer Structure
```
Concept: 2D array of pixels
Size: width × height × bytes_per_pixel

Common formats:
- RGB24:  3 bytes per pixel (R, G, B)
- RGBA32: 4 bytes per pixel (R, G, B, A)
- RGB565: 2 bytes per pixel (5-6-5 bit format)
```

### Key Functions to Implement
1. **Framebuffer Creation**
   - Allocate: `width * height * 4` bytes (for RGBA)
   - Initialize to black or a clear color

2. **SetPixel**
   ```
   setPixel(x, y, color):
     - Bounds check: if x or y out of range, return
     - Calculate index: idx = (y * width + x) * 4
     - framebuffer[idx+0] = color.r
     - framebuffer[idx+1] = color.g
     - framebuffer[idx+2] = color.b
     - framebuffer[idx+3] = color.a
   ```

3. **Clear Framebuffer**
   - Fill entire buffer with a single color
   - Useful for clearing between frames

### First Test
```
for y in 0..height:
  for x in 0..width:
    r = (x / width) * 255
    g = (y / height) * 255
    b = 128
    setPixel(x, y, RGB(r, g, b))
```
Expected: A gradient from black to cyan

---

## Phase 2 Deep Dive: 2D Primitives

### Bresenham's Line Algorithm
```
Key concept: Draw lines using only integer arithmetic

drawLine(x0, y0, x1, y1, color):
  dx = abs(x1 - x0)
  dy = abs(y1 - y0)
  sx = 1 if x0 < x1 else -1
  sy = 1 if y0 < y1 else -1
  err = dx - dy

  while true:
    setPixel(x0, y0, color)
    if x0 == x1 and y0 == y1: break

    e2 = 2 * err
    if e2 > -dy:
      err -= dy
      x0 += sx
    if e2 < dx:
      err += dx
      y0 += sy
```

### Triangle Outline
```
drawTriangleOutline(v0, v1, v2, color):
  drawLine(v0.x, v0.y, v1.x, v1.y, color)
  drawLine(v1.x, v1.y, v2.x, v2.y, color)
  drawLine(v2.x, v2.y, v0.x, v0.y, color)
```

### Testing 2D
- Draw a coordinate axis (red X, green Y)
- Draw a grid
- Draw overlapping shapes to test rendering order

---

## Phase 3 Deep Dive: 3D Math

### Essential Data Structures

#### Vector3
```
struct Vec3:
  float x, y, z

  Vec3 + Vec3  → addition
  Vec3 - Vec3  → subtraction
  Vec3 * float → scalar multiplication
  dot(Vec3)    → dot product (returns float)
  cross(Vec3)  → cross product (returns Vec3)
  length()     → magnitude
  normalize()  → unit vector
```

#### Matrix4x4
```
struct Mat4:
  float m[16]  // or m[4][4]

  Identity matrix:
  [1 0 0 0]
  [0 1 0 0]
  [0 0 1 0]
  [0 0 0 1]

  Operations:
  - multiply(Mat4) → matrix multiplication
  - multiply(Vec4) → transform vector
```

### Transformation Matrices

#### Translation
```
translate(tx, ty, tz):
  [1  0  0  tx]
  [0  1  0  ty]
  [0  0  1  tz]
  [0  0  0  1 ]
```

#### Rotation (around Y-axis)
```
rotateY(angle):
  c = cos(angle)
  s = sin(angle)

  [ c  0  s  0]
  [ 0  1  0  0]
  [-s  0  c  0]
  [ 0  0  0  1]
```

#### Scale
```
scale(sx, sy, sz):
  [sx  0   0  0]
  [ 0  sy  0  0]
  [ 0   0  sz 0]
  [ 0   0   0 1]
```

### Projection Matrix (Perspective)

```
perspective(fov, aspect, near, far):
  f = 1 / tan(fov / 2)

  [f/aspect  0   0                          0              ]
  [0         f   0                          0              ]
  [0         0   (far+near)/(near-far)      2*far*near/(near-far)]
  [0         0   -1                         0              ]
```

### View Matrix (Camera)
```
lookAt(eye, target, up):
  1. Calculate forward = normalize(target - eye)
  2. Calculate right = normalize(cross(forward, up))
  3. Calculate up_new = cross(right, forward)
  4. Build matrix from right, up_new, -forward, and eye position
```

### 3D to 2D Projection Pipeline
```
For each vertex:
  1. World transform:    world_pos = model_matrix * vertex
  2. View transform:     view_pos = view_matrix * world_pos
  3. Projection:         clip_pos = projection_matrix * view_pos
  4. Perspective divide: ndc = clip_pos.xyz / clip_pos.w
  5. Viewport transform: screen_x = (ndc.x + 1) * width / 2
                         screen_y = (1 - ndc.y) * height / 2
```

---

## Phase 4 Deep Dive: Triangle Rasterization

### Edge Function Method (Recommended)

```
Key concept: Use cross products to determine if point is inside triangle

edgeFunction(a, b, p):
  return (p.x - a.x) * (b.y - a.y) - (p.y - a.y) * (b.x - a.x)

fillTriangle(v0, v1, v2, color):
  # Find bounding box
  min_x = max(0, floor(min(v0.x, v1.x, v2.x)))
  max_x = min(width-1, ceil(max(v0.x, v1.x, v2.x)))
  min_y = max(0, floor(min(v0.y, v1.y, v2.y)))
  max_y = min(height-1, ceil(max(v0.y, v1.y, v2.y)))

  # Calculate area (for barycentric coordinates)
  area = edgeFunction(v0, v1, v2)

  # Scan bounding box
  for y in min_y..max_y:
    for x in min_x..max_x:
      p = Point(x + 0.5, y + 0.5)  # Pixel center

      # Calculate barycentric coordinates
      w0 = edgeFunction(v1, v2, p)
      w1 = edgeFunction(v2, v0, p)
      w2 = edgeFunction(v0, v1, p)

      # Check if point is inside
      if w0 >= 0 and w1 >= 0 and w2 >= 0:
        setPixel(x, y, color)
```

### Barycentric Coordinates
- w0, w1, w2 are weights for vertices v0, v1, v2
- Normalized: λ0 = w0/area, λ1 = w1/area, λ2 = w2/area
- Used to interpolate vertex attributes (colors, UVs, normals)
- `value = λ0 * value0 + λ1 * value1 + λ2 * value2`

---

## Phase 5 Deep Dive: Depth Buffer

### Z-Buffer Implementation
```
Initialize:
  depth_buffer = array of width × height floats
  fill with infinity or 1.0 (farthest depth)

During rasterization:
  for each pixel (x, y) in triangle:
    # Calculate depth at this pixel (interpolate vertex depths)
    depth = λ0 * v0.z + λ1 * v1.z + λ2 * v2.z

    # Depth test
    if depth < depth_buffer[x, y]:
      depth_buffer[x, y] = depth
      setPixel(x, y, color)
    # else: pixel is behind, skip
```

### Perspective-Correct Interpolation
```
Problem: Linear interpolation in screen space is wrong for 3D attributes

Solution: Interpolate in 3D, then divide by interpolated depth

For attribute A (like UV or color):
  1. At vertices, compute: A_over_w = A / vertex.w
  2. Interpolate: value_over_w = λ0 * A0/w0 + λ1 * A1/w1 + λ2 * A2/w2
  3. Interpolate 1/w: one_over_w = λ0/w0 + λ1/w1 + λ2/w2
  4. Recover: A = value_over_w / one_over_w
```

---

## Phase 6 Deep Dive: Texturing

### Texture Class
```
struct Texture:
  width, height: int
  pixels: array of colors

  sample(u, v, filter_mode):
    # u, v are in range [0, 1]
    # Convert to pixel coordinates
    x = u * (width - 1)
    y = v * (height - 1)

    if filter_mode == NEAREST:
      return pixels[round(y) * width + round(x)]

    elif filter_mode == BILINEAR:
      # Get 4 surrounding pixels
      x0, y0 = floor(x), floor(y)
      x1, y1 = x0 + 1, y0 + 1

      # Get fractional parts
      fx, fy = x - x0, y - y0

      # Bilinear interpolation
      c00 = getPixel(x0, y0)
      c10 = getPixel(x1, y0)
      c01 = getPixel(x0, y1)
      c11 = getPixel(x1, y1)

      c0 = lerp(c00, c10, fx)
      c1 = lerp(c01, c11, fx)
      return lerp(c0, c1, fy)
```

### Textured Triangle Rendering
```
For each pixel in triangle:
  # Interpolate UV coordinates (perspective-correct!)
  u = perspectiveCorrectInterpolate(u0, u1, u2, λ0, λ1, λ2, w0, w1, w2)
  v = perspectiveCorrectInterpolate(v0, v1, v2, λ0, λ1, λ2, w0, w1, w2)

  # Sample texture
  color = texture.sample(u, v)

  # Set pixel (with depth test)
  if depth < depth_buffer[x, y]:
    depth_buffer[x, y] = depth
    setPixel(x, y, color)
```

---

## Phase 7 Deep Dive: Lighting

### Diffuse Lighting (Lambertian)
```
Key concept: Surface brightness depends on angle to light

diffuse = max(0, dot(normal, light_dir)) * light_color * surface_color

Where:
  - normal: surface normal (normalized)
  - light_dir: direction from surface to light (normalized)
  - dot product gives cosine of angle
  - max(0, ...) ensures no negative lighting (backfaces)
```

### Specular Lighting (Blinn-Phong)
```
Key concept: Shiny highlights depend on view angle

half_vector = normalize(light_dir + view_dir)
specular = pow(max(0, dot(normal, half_vector)), shininess) * light_color

Where:
  - half_vector: halfway between light and view directions
  - shininess: controls highlight size (8-256 typical)
```

### Combined Lighting Model
```
final_color = ambient + diffuse + specular

Where:
  ambient  = ambient_strength * light_color * surface_color
  diffuse  = (as above)
  specular = (as above)
```

### Per-Vertex vs Per-Pixel Lighting

**Gouraud Shading (Per-Vertex)**
```
1. Calculate lighting at each vertex
2. Interpolate colors across triangle
3. Faster, but less accurate
```

**Phong Shading (Per-Pixel)**
```
1. Interpolate normals across triangle
2. Calculate lighting at each pixel
3. Slower, but much better quality
```

---

## Phase 8 Deep Dive: Optimizations

### Backface Culling
```
For each triangle:
  # Calculate face normal
  edge1 = v1 - v0
  edge2 = v2 - v0
  face_normal = cross(edge1, edge2)

  # Calculate view direction
  view_dir = camera_pos - v0

  # If facing away, don't render
  if dot(face_normal, view_dir) <= 0:
    skip this triangle
```

### Frustum Culling
```
Check if object's bounding box intersects view frustum
If not, skip entire object
Saves transformation and rasterization time
```

### Multi-threading
```
Split screen into tiles or rows
Each thread renders subset of pixels
Requires careful synchronization for shared data
```

---

## Debugging Tools to Build

1. **Wireframe Toggle** - See triangle edges
2. **Normal Visualization** - Render normals as colors
3. **UV Visualization** - Show UV coords as red/green
4. **Depth Visualization** - Render depth buffer as grayscale
5. **Bounding Box Display** - Show object bounds
6. **Frame Capture** - Save frame as image file

---

## Performance Tips

### Profiling First
- Measure where time is spent
- Rasterization usually dominates
- Don't optimize math until it's proven slow

### Optimization Techniques
1. **Integer arithmetic** where possible
2. **SIMD** for vector operations (4 floats at once)
3. **Cache-friendly** memory access patterns
4. **Early-out** tests (bounding box, depth test)
5. **Fixed-point math** for embedded systems

### Common Bottlenecks
- Per-pixel operations (minimize work here)
- Texture sampling (cache misses)
- Overdraw (rendering same pixel many times)

---

## Common Pitfalls

1. **Forgetting perspective divide** - Objects will look wrong
2. **Wrong triangle winding** - Backface culling removes wrong faces
3. **Not normalizing vectors** - Lighting will be wrong
4. **Integer truncation** - Use proper rounding
5. **Off-by-one errors** - Gaps between triangles
6. **Forgetting to clear buffers** - Previous frame bleeds through
7. **Wrong matrix multiplication order** - Transforms don't compose correctly

---

## Testing Strategy

### Unit Tests
- Vector/matrix operations
- Edge functions with known triangles
- Barycentric coordinate calculation

### Visual Tests
- Render reference shapes (cube, sphere, teapot)
- Compare against OpenGL/DirectX screenshots
- Check edge cases (very large/small triangles)

### Validation
- Check for gaps between triangles
- Verify z-fighting doesn't occur
- Test texture wrapping modes
- Verify lighting matches reference images

---

## Next Steps After Completion

1. **Optimization** - Make it fast!
2. **Shader System** - Programmable vertex/pixel shaders
3. **Ray Tracing** - Different rendering paradigm
4. **GPU Implementation** - Port to OpenGL/Vulkan compute
5. **Game Engine** - Build complete engine around renderer

---

Remember: This is a learning project. Write clean, understandable code first. Optimize later. Comment generously. You're building the foundation for understanding all modern 3D graphics!
