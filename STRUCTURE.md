# PBD-X Project Structure

This project has been refactored to support both 2D and 3D physics-based deformation simulations. The codebase is now organized into two separate folder structures for clean separation of concerns.

## Directory Structure

```
src/
├── 2d/                          # 2D Simulation Code
│   ├── core/                    # Core classes for 2D
│   │   ├── Vector2D.h          # 2D vector math
│   │   ├── PointMass.h/cpp     # 2D point mass
│   │   └── Spring.h/cpp        # 2D spring physics
│   ├── gui/                     # 2D Graphics
│   │   ├── GLFWContext.h/cpp   # Window management
│   │   ├── Shader.h/cpp        # OpenGL shaders
│   │   ├── OpenGLRenderer2D.h/cpp     # 2D rendering
│   │   └── OpenGLApplication2D.h/cpp  # 2D application
│   ├── objects/                 # 2D Physics Objects
│   │   ├── ClothObject.h/cpp   # Cloth simulation
│   │   └── RopeObject.h/cpp    # Rope simulation
│   ├── simulation/              # 2D Simulation Engine
│   │   └── Simulation.h/cpp    # 2D physics updates
│   └── utils/                   # 2D Constants
│       └── Constants.h
│
├── 3d/                          # 3D Simulation Code
│   ├── core/                    # Core classes for 3D
│   │   ├── Vector3D.h          # 3D vector math (with cross product)
│   │   ├── PointMass.h/cpp     # 3D point mass
│   │   └── Spring.h/cpp        # 3D spring physics
│   ├── gui/                     # 3D Graphics
│   │   ├── GLFWContext.h/cpp   # Window management
│   │   ├── Shader.h/cpp        # OpenGL shaders
│   │   ├── OpenGLRenderer3D.h/cpp     # 3D rendering with perspective
│   │   └── OpenGLApplication3D.h/cpp  # 3D application
│   ├── objects/                 # 3D Physics Objects
│   │   ├── ClothObject.h/cpp   # 3D cloth simulation
│   │   └── RopeObject.h/cpp    # 3D rope simulation
│   ├── simulation/              # 3D Simulation Engine
│   │   └── Simulation.h/cpp    # 3D physics updates
│   └── utils/                   # 3D Constants
│       └── Constants.h
│
├── main_2d.cpp                  # Entry point for 2D mode
├── main_3d.cpp                  # Entry point for 3D mode
└── CMakeLists.txt               # Build configuration
```

## Building the Project

### Build 2D Version (Default)

```bash
cd cmake-build-debug
cmake ..
make
./pbd-x
```

Or explicitly set 2D mode:

```bash
cmake -DBUILD_MODE=2D ..
make
./pbd-x
```

### Build 3D Version

```bash
cmake -DBUILD_MODE=3D ..
make
./pbd-x
```

## Controls

### 2D Mode
- **ESC**: Exit application
- **SPACE**: Pause/Resume simulation
- **G**: Toggle gravity
- **W**: Toggle wind force
- **Left Mouse Drag**: Pan camera
- **Mouse Wheel**: Zoom in/out
- **Z**: Zoom in
- **X**: Zoom out
- **[**: Decrease simulation speed
- **]**: Increase simulation speed

### 3D Mode
- **ESC**: Exit application
- **SPACE**: Pause/Resume simulation
- **G**: Toggle gravity
- **W**: Toggle wind force
- **Left Mouse Drag**: Rotate camera
- **Mouse Wheel**: Zoom in/out
- **Arrow Keys**: Pan camera
- **Z**: Zoom out
- **X**: Zoom in
- **[**: Decrease simulation speed
- **]**: Increase simulation speed

## Key Differences Between 2D and 3D

### Vector Math
- **2D**: `Vector2D` with x, y components
- **3D**: `Vector3D` with x, y, z components and cross product support

### Point Mass
- **2D**: Initialized with `PointMass(mass, x, y)`
- **3D**: Initialized with `PointMass(mass, x, y, z)`

### Objects
- **2D**: Cloth and Rope with 2D positioning
- **3D**: Cloth and Rope with 3D positioning

### Rendering
- **2D**: Orthographic projection with pan and zoom
- **3D**: Perspective projection with rotation, pan, and zoom
- **3D**: Uses depth testing for proper rendering

## Adding New Features

When adding features, consider if they need to be:

1. **In both 2D and 3D**: Add to respective folders
2. **2D only**: Add to `src/2d/`
3. **3D only**: Add to `src/3d/`

## Compilation Notes

- Both versions require the same dependencies: **GLFW3**, **GLM**, **GLAD**
- The CMakeLists.txt automatically configures include directories based on BUILD_MODE
- Each version compiles independently with no cross-dependencies

## Future Enhancements

- [ ] Shared base classes for common functionality
- [ ] Plugin system for custom physics objects
- [ ] Multi-threaded simulation
- [ ] Advanced 3D rendering (lighting, shadows, materials)
- [ ] Network simulation sharing
