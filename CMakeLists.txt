cmake_minimum_required(VERSION 3.24)
project(OpenGL_ECS_Engine)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MODULE_PATH /usr/local/lib/cmake)
set(CMAKE_PREFIX_PATH /usr/local/lib/cmake/glfw3)

find_package (PkgConfig REQUIRED)
find_package (OpenGL REQUIRED)
find_package (glfw3 REQUIRED)

include_directories(/usr/local/include glad)
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(include)
include_directories(include/glad)
include_directories(/usr/include/GLFW)
include_directories(include/glm)
include_directories(include/glm/detail)
include_directories(include/glm/ext)
include_directories(include/glm/gtc)
include_directories(include/glm/gtx)
include_directories(include/glm/simd)
include_directories(include/KHR)


file(GLOB SOURCE_FILES
        "src/*.hpp"
        "src/*.cpp"
        )

add_compile_options(-lglfw)
add_executable(OpenGL_ECS_Engine src/glad.c ${SOURCE_FILES})
target_link_libraries(OpenGL_ECS_Engine ${GLFW3_LIBRARY} glfw)