cmake_minimum_required(VERSION 3.10.0)
project(MonotileVisualizer VERSION 0.1.0 LANGUAGES C CXX)

include_directories(include utils)
add_executable(MonotileVisualizer 
    src/main.cpp 
    src/glad.c 
    src/Game.cpp 
    src/Renderer.cpp
    src/Monotile.cpp
    utils/glslprogram.cpp
    utils/GLUtils.cpp)

target_link_libraries(MonotileVisualizer glfw)
