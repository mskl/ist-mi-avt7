cmake_minimum_required(VERSION 3.14)

project(AVT7)
set(CMAKE_CXX_STANDARD 11)

# Include all of the files into the project
file(GLOB_RECURSE SOURCES src/*.h src/*.cpp src/Camera.h)
add_executable(AVT7 ${SOURCES} src/PerspectiveCamera.h)

# Linking might be different on other OS
target_link_libraries(AVT7 -lm -lGL -L/usr/X11R6/lib -lGLU -lglut -lGLEW)
