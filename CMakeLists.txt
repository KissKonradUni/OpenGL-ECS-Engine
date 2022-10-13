cmake_minimum_required(VERSION 3.24)
project(OpenGL_ECS_Engine)

set(BINARY ${CMAKE_PROJECT_NAME})

find_package(OpenGL REQUIRED)
find_package (PkgConfig REQUIRED)
find_package (OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package (glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules/)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

file(GLOB SOURCE_FILES
        "src/*.hpp"
        "src/*.cpp"
        )

if(CMAKE_CXX_COMPILER_ID MATCHES "CLANG")
        target_compile_options(${BINARY} PRIVATE -fp:fast)
        target_compile_options(${BINARY} PRIVATE -march:native)
endif() 

add_executable(${BINARY} src/glad.c ${SOURCE_FILES})
target_link_libraries(${BINARY} PRIVATE glfw glm::glm OpenGL::GL glad::glad)

file(COPY res DESTINATION .)