cmake_minimum_required(VERSION 3.14)

project(AVT7)
set(CMAKE_CXX_STANDARD 11)

# Include all of the files into the project
file(GLOB_RECURSE SOURCES src/*.h src/*.cpp src/libs/TGA.cpp src/libs/TGA.h)
add_executable(AVT7 ${SOURCES})

target_link_libraries(AVT7 -lm -lGL -L/usr/X11R6/lib -lGLU -lglut -lGLEW -ltinyxml -lIL -lILU)