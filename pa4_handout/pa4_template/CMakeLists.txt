cmake_minimum_required(VERSION 3.16)
project(pa4)
set(CMAKE_CXX_STANDARD 17)

# packages to find

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

# all sources

set(GLAD
        include/glad/glad.h
        include/KHR/khrplatform.h
        src/glad/glad.c
        )

set(shader
        include/util/Camera.h
        include/util/Shader.h
        )

set(SHAPE
        include/shape/Cube.h
        include/shape/Dodecahedron.h
        include/shape/Icosahedron.h
        include/shape/Shape.h
        include/shape/Tetrahedron.h
        include/shape/Octahedron.h
        )

set(ALL_SOURCE_FILES
        ${GLAD}
        ${SHADER}
        ${SHAPE}
        src/main.cpp
        )

set(ALL_INCLUDE_DIRS
        ${GLEW_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIR}
        ${OPENGL_EGL_INCLUDE_DIRS}
        include
        )

set(ALL_LIBRARIES
        dl
        GLEW::GLEW
        glfw
        OpenGL::GL
        OpenGL::OpenGL
        OpenGL::GLU
        OpenGL::GLX
        OpenGL::EGL
        pthread
        )

# all definitions and options

set(ALL_COMPILE_DEFS

        )

set(ALL_COMPILE_OPTS
        -Wpessimizing-move
        -Wredundant-move
        )

# executable target

set(EXECUTABLE pa4)
add_executable(${EXECUTABLE} ${ALL_SOURCE_FILES})
target_compile_definitions(${EXECUTABLE} PUBLIC ${ALL_COMPILE_DEFS})
target_compile_options(${EXECUTABLE} PUBLIC ${ALL_COMPILE_OPTS})
target_include_directories(${EXECUTABLE} PUBLIC ${ALL_INCLUDE_DIRS})
target_link_libraries(${EXECUTABLE} ${ALL_LIBRARIES})
