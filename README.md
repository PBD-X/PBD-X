# PBD-X
Hybridization of Soft Body Deformation Models Using Position-Based Dynamics and Mass-Spring Methods

## Install Guide

### Prerequisites

- C++ 17 complier
- vcpkg
- cmake

### Installation

1. Clone source code

`git clone https://github.com/PBD-X/PBD-X.git`

2. Edit cmake file to run proper version
3. Build and run using cmake
   1. Vscode:
        - `cmake ../my/project -DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>/scripts/buildsystems/vcpkg.cmake`
   2. Mingw:
        - `cmake ../my/project -DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>/scripts/buildsystems/vcpkg.cmake -DVPKG_TARGET_TRIPLET=x64-mingw-dynamic -DVPKG_HOST_TRIPLET=x64-mingw-dynamic`

## Project Structure

```
PBD-X/
├───src/
│   ├───2d/
│   │   ├───core/
│   │   │   ├───PointMass.cpp
│   │   │   ├───PointMass.h
│   │   │   ├───Spring.cpp
│   │   │   ├───Spring.h
│   │   │   └───Vector2D.h
│   │   ├───gui/
│   │   │   ├───GLFWContext.cpp
│   │   │   ├───GLFWContext.h
│   │   │   ├───OpenGLApplication2D.cpp
│   │   │   ├───OpenGLApplication2D.h
│   │   │   ├───OpenGLRenderer2D.cpp
│   │   │   ├───OpenGLRenderer2D.h
│   │   │   ├───Shader.cpp
│   │   │   └───Shader.h
│   │   ├───objects/
│   │   │   ├───ClothObject.cpp
│   │   │   ├───ClothObject.h
│   │   │   ├───RopeObject.cpp
│   │   │   └───RopeObject.h
│   │   ├───simulation/
│   │   │   ├───Simulation.cpp
│   │   │   └───Simulation.h
│   │   └───utils/
│   │       └───Constants.h
│   ├───3d/
│   │   ├───core/
│   │   │   ├───PointMass.cpp
│   │   │   ├───PointMass.h
│   │   │   ├───Spring.cpp
│   │   │   ├───Spring.h
│   │   │   └───Vector3D.h
│   │   ├───gui/
│   │   │   ├───GLFWContext.cpp
│   │   │   ├───GLFWContext.h
│   │   │   ├───OpenGLApplication3D.cpp
│   │   │   ├───OpenGLApplication3D.h
│   │   │   ├───OpenGlRenderer3D.cpp
│   │   │   ├───OpenGLRenderer3D.h
│   │   │   ├───Shader.cpp
│   │   │   └───Shader.h
│   │   ├───objects/
│   │   │   ├───ClothObject.cpp
│   │   │   ├───ClothObject.h
│   │   │   ├───RopeObject.cpp
│   │   │   └───RopeObject.h
│   │   ├───simulation/
│   │   │   ├───Simulation.cpp
│   │   │   └───Simulation.h
│   │   └───utils/
│   │       └───Simulation.h
│   ├───main_2d.cpp
│   └───main_3d.cpp
├───.gitignore
├───CMakeLists.txt
├───LICENSE
├───README.md
├───vcpkg.json
└───vcpkg-configuration.json