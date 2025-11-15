# Quick Start Guide

## Project Structure Explanation

Your PBD-X project has been successfully refactored to support **both 2D and 3D** simulations with completely separate codebases.

```
PBD-X-Kevin/
├── src/
│   ├── 2d/              ← 2D Simulation (8x8 cloth, 10-point rope)
│   ├── 3d/              ← 3D Simulation (8x8 cloth, 10-point rope)
│   ├── main_2d.cpp      ← Entry point for 2D
│   └── main_3d.cpp      ← Entry point for 3D
├── CMakeLists.txt       ← Updated to support both modes
└── [old files]          ← Can be deleted (replaced by 2d/ and 3d/)
```

## Building

### Build 2D Version (Default)
```powershell
cd cmake-build-debug
cmake ..
cmake --build . --config Debug
```

### Build 3D Version
```powershell
cd cmake-build-debug
cmake -DBUILD_MODE=3D ..
cmake --build . --config Debug
```

## Key Changes

| Aspect | 2D | 3D |
|--------|----|----|
| **Vector** | `Vector2D(x, y)` | `Vector3D(x, y, z)` |
| **Point Mass** | `PointMass(m, x, y)` | `PointMass(m, x, y, z)` |
| **Rendering** | Orthographic (2D view) | Perspective (3D view) |
| **Camera Control** | Pan + Zoom | Rotate + Pan + Zoom |
| **Depth Testing** | Disabled | Enabled |

## Controls

### 2D Mode
- **Left Mouse Drag**: Pan camera
- **Mouse Wheel or Z/X**: Zoom
- **SPACE**: Pause/Resume
- **G**: Toggle gravity
- **W**: Toggle wind

### 3D Mode
- **Left Mouse Drag**: Rotate camera
- **Arrow Keys**: Pan camera
- **Mouse Wheel or Z/X**: Zoom
- **SPACE**: Pause/Resume
- **G**: Toggle gravity
- **W**: Toggle wind

## File Organization

### 2D Source Files (src/2d/)
```
src/2d/
├── core/
│   ├── Vector2D.h
│   ├── PointMass.h/cpp
│   └── Spring.h/cpp
├── gui/
│   ├── GLFWContext.h/cpp
│   ├── Shader.h/cpp
│   ├── OpenGLRenderer2D.h/cpp
│   └── OpenGLApplication2D.h/cpp
├── objects/
│   ├── ClothObject.h/cpp
│   └── RopeObject.h/cpp
├── simulation/
│   └── Simulation.h/cpp
└── utils/
    └── Constants.h
```

### 3D Source Files (src/3d/)
Same structure as 2D, but with `Vector3D` and `OpenGLRenderer3D`

## Features Implemented

✅ **Completed:**
- Separated 2D and 3D codebases
- 3D vector math with cross product
- 3D point mass and spring physics
- 3D cloth and rope simulations
- 3D perspective rendering with proper projection
- Camera rotation system for 3D
- Depth testing for 3D rendering
- Both versions fully functional and independent

## Next: Clean Up Old Files

The following files in `src/` root are **no longer used** and can be deleted:
```
src/
├── core/            ← DELETE (moved to src/2d/core and src/3d/core)
├── gui/             ← DELETE (moved to src/2d/gui and src/3d/gui)
├── objects/         ← DELETE (moved to src/2d/objects and src/3d/objects)
├── simulation/      ← DELETE (moved to src/2d/simulation and src/3d/simulation)
├── utils/           ← DELETE (moved to src/2d/utils and src/3d/utils)
└── main.cpp         ← DELETE (replaced by main_2d.cpp and main_3d.cpp)
```

## Troubleshooting

**Q: How do I switch between 2D and 3D?**
A: Change the build mode in CMake:
```powershell
cmake -DBUILD_MODE=3D ..  # For 3D
cmake -DBUILD_MODE=2D ..  # For 2D (default)
```

**Q: Do I need to install new dependencies?**
A: No, both use the same dependencies (GLFW3, GLM, GLAD).

**Q: Can I run 2D and 3D at the same time?**
A: You can build separate executables by creating two build directories, or modifying the CMake to support both in one executable.

## Documentation Files

- `STRUCTURE.md` - Detailed project structure and architecture
- `REFACTORING.md` - Summary of refactoring changes
- This file - Quick start guide

Enjoy your refactored 2D and 3D PBD-X simulator!
