cmake_minimum_required(VERSION 3.20)

project(ProyectoMedioSemestre LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(ProyectoMedioSemestre
    src/obj_helpers/main.cpp
    src/obj_helpers/Scene.cpp
    src/obj_helpers/WindowGL.cpp
    src/models/ImportedModel.cpp
    src/models/Grid.cpp
    src/obj_helpers/ShaderProgram.cpp
    src/camera/OrbitCamera.cpp
)

target_link_libraries(ProyectoMedioSemestre PRIVATE
    OpenGL::GL
    glfw
    GLEW::GLEW
    glm::glm
)

target_compile_definitions(ProyectoMedioSemestre PRIVATE
    PROJECT_ROOT="${CMAKE_CURRENT_SOURCE_DIR}"
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
