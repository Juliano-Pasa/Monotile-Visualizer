cmake_minimum_required(VERSION 3.10.0)
project(openGlTesting VERSION 0.1.0 LANGUAGES C CXX)

include_directories(include)
add_executable(openGlTesting src/main.cpp src/glad.c)

target_link_libraries(openGlTesting glfw)
