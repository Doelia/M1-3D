cmake_minimum_required(VERSION 2.8.4)
project(M1_3D)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    lib/Coord.cpp
    lib/Coord.h
    lib/GlutIncluder.h
    lib/Include.h
    lib/Point.cpp
    lib/Point.h
    lib/Vector.cpp
    lib/Vector.h
    TP1/Coord.cpp
    TP1/Coord.h
    TP1/main.cpp
    TP1/opengl.cpp
    TP1/Point.cpp
    TP1/Point.h
    TP1/Vector.cpp
    TP1/Vector.h
    TP2/Coord.cpp
    TP2/Coord.h
    TP2/GlutIncluder.h
    TP2/opengl.cpp
    TP2/Point.cpp
    TP2/Point.h
    TP2/ToolsOpenGL.h
    TP2/Vector.cpp
    TP2/Vector.h
    TP3/opengl.cpp
    TP3/ToolsOpenGL.h
    TP4/main.h
    TP4/opengl.cpp)

add_executable(M1_3D ${SOURCE_FILES})