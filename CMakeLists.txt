cmake_minimum_required(VERSION 3.20)

# For clangd to scan for include dirs
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
# Build type
#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Release)

# set project name
project(opengl-cube VERSION 0.1)

# add OpenGL libraries
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

# add executable
add_executable(cube "${PROJECT_SOURCE_DIR}/src/main.c")
target_link_libraries(cube ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
