cmake_minimum_required(VERSION 3.15)
project(PBD-X VERSION 1.0.0 LANGUAGES CXX)

set(PROJECT_DESCRIPTION "Hybridization of Soft Body Deformation Models Using Position-Based Dynamics and Mass-Spring Methods")
set(PROJECT_HOMEPAGE_URL "https://github.com/PBD-X/PBD-X")
set(PROJECT_LICENSE "MIT")
set(PROJECT_LICENSE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(
        ${CMAKE_SOURCE_DIR}/include
)

set(SOURCES
    src/main.cpp
)

set(HEADERS
    src/math/Vector2.h
    src/math/Vector3.h
)

add_executable(pbd-x src/main.cpp
        src/math/Vector2.h
        src/math/Vector3.h)
