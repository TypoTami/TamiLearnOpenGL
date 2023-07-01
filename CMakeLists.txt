cmake_minimum_required(VERSION 3.25)
project(TamiOpenGL)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-static")

set(GLFW_USE_WAYLAND on)

add_executable(
        TamiOpenGL
        main.cpp
        glad/src/glad.c
        shader.h stb_image.cpp)

#find_package(glm CONFIG REQUIRED)

add_subdirectory(glm)
add_subdirectory(glfw)
include_directories(glad/include)

target_link_libraries(TamiOpenGL PRIVATE glfw glm::glm)