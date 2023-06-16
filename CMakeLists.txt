cmake_minimum_required(VERSION 3.25)
project(TamiOpenGL)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(glfw)
include_directories(glad/include)

add_executable(
        TamiOpenGL
        main.cpp
        glad/src/glad.c
        shader.h stb_image.cpp)

target_link_libraries(TamiOpenGL glfw)