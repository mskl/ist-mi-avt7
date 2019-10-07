cmake_minimum_required(VERSION 3.14)
project(AVT7)

set(CMAKE_CXX_STANDARD 11)

add_definitions(-DTIXML_USE_STL -DFREEGLUT_STATIC -D_CRT_SECURE_NO_WARNINGS)

file(GLOB SOURCES labs/matyas/minimal/*.h labs/matyas/minimal/*.cpp)

add_executable(AVT7 ${SOURCES})
target_link_libraries(AVT7 -lm -lGL -L/usr/X11R6/lib -lGLU -lglut -lGLEW)
