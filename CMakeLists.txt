cmake_minimum_required(VERSION 3.10)
project(PhongTro3D_Expert)

set(CMAKE_CXX_STANDARD 17)
add_definitions(-DGLFW_INCLUDE_NONE)

# Include Directories
include_directories(include)

# Link Directories for GLFW
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/external/glfw-3.4.bin.WIN64/lib-vc2022")

# Sources
set(SOURCES
    src/main.cpp
    src/glad.c
    src/Scene.cpp
    src/Object3D.cpp
)

add_executable(PhongTro3D ${SOURCES})

# Libraries
target_link_libraries(PhongTro3D opengl32 glfw3)

# Shaders and Resources
# (Optional: Copy shaders to build folder)
