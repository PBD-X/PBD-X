cmake_minimum_required(VERSION 3.10)
project(PBD-X LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

# 2D Sources
set (2D_SOURCES
        src/2d/core/PointMass.cpp
        src/2d/core/Spring.cpp
        src/2d/simulation/Simulation.cpp
        src/2d/objects/ClothObject.cpp
        src/2d/objects/RopeObject.cpp
        src/2d/gui/GLFWContext.cpp
        src/2d/gui/Shader.cpp
        src/2d/gui/OpenGLRenderer2D.cpp
        src/2d/gui/OpenGLApplication2D.cpp
)

# 3D Sources
set (3D_SOURCES
        src/3d/core/PointMass.cpp
        src/3d/core/Spring.cpp
        src/3d/simulation/Simulation.cpp
        src/3d/objects/ClothObject.cpp
        src/3d/objects/RopeObject.cpp
        src/3d/gui/GLFWContext.cpp
        src/3d/gui/Shader.cpp
        src/3d/gui/OpenGLRenderer3D.cpp
        src/3d/gui/OpenGLApplication3D.cpp
)

# Default to 2D mode. Change to 3D by setting -DBUILD_MODE=3D
if(NOT DEFINED BUILD_MODE)
    set(BUILD_MODE "3D")
endif()

if(BUILD_MODE STREQUAL "2D")
    message(STATUS "Building 2D Simulation")
    set(SOURCES src/main_2d.cpp ${2D_SOURCES})
    set(INCLUDE_DIRS src/2d/core src/2d/simulation src/2d/objects src/2d/utils src/2d/gui)
elseif(BUILD_MODE STREQUAL "3D")
    message(STATUS "Building 3D Simulation")
    set(SOURCES src/main_3d.cpp ${3D_SOURCES})
    set(INCLUDE_DIRS src/3d/core src/3d/simulation src/3d/objects src/3d/utils src/3d/gui)
else()
    message(FATAL_ERROR "BUILD_MODE must be 2D or 3D")
endif()

include_directories(${INCLUDE_DIRS})

add_executable(pbd-x ${SOURCES})

target_link_libraries(pbd-x PRIVATE glm::glm glfw glad::glad)