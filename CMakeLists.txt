cmake_minimum_required(VERSION 3.14)
project(AVT7)

set(CMAKE_CXX_STANDARD 11)

add_executable(AVT7
        labs/matyas/02/AVTmathLib.h labs/matyas/02/AVTmathLib.cpp
        labs/matyas/02/basic_geometry.cpp labs/matyas/02/basic_geometry.h
        labs/matyas/02/vsShaderLib.cpp labs/matyas/02/vsShaderLib.h)
target_link_libraries(AVT7 -lm -lGL -L/usr/X11R6/lib -lGLU -lglut -lGLEW)
