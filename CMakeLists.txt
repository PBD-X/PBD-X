cmake_minimum_required(VERSION 3.10)
project(PBD-X LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

set (SOURCES
        src/main.cpp
        src/core/PointMass.cpp
        src/core/Spring.cpp
        src/simulation/Simulation.cpp
        src/objects/ClothObject.cpp
        src/objects/RopeObject.cpp
        src/gui/GLFWContext.cpp
        src/gui/GLFWContext.h
        src/gui/Shader.cpp
        src/gui/Shader.h
        src/gui/OpenGLRenderer.cpp
        src/gui/OpenGLRenderer.h
        src/gui/OpenGLApplication.cpp
        src/gui/OpenGLApplication.h
)

include_directories(
        src/
        src/core/
        src/simulation/
        src/objects/
        src/utils/
        src/gui/
)

add_executable(pbd-x ${SOURCES})

target_link_libraries(pbd-x PRIVATE glm::glm glfw glad::glad)