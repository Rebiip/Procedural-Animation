cmake_minimum_required(VERSION 3.28)
project(procedural_animation)
find_package(freetype)
find_package(glfw3)
find_package(OpenGL REQUIRED)
find_package(glad)
find_package(glm)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(procedural_animation main.cpp
        src/application/SceneController.cpp
        src/application/SceneController.h
        src/application/ports/Renderer.h
        src/infrastructure/opengl/OpenGlRenderer.cpp
        src/infrastructure/opengl/OpenGlRenderer.h
        src/domain/entities/Renderable.h
        src/domain/entities/Camera.h
        src/domain/entities/Window.h
        src/infrastructure/opengl/OpenGlWindow.cpp
        src/infrastructure/opengl/OpenGlWindow.h
        src/infrastructure/opengl/shaders/Shader.cpp
        src/presentation/ProcessInputController.cpp
        src/presentation/ProcessInputController.h
        src/presentation/dtos/MouseButton.h
        src/presentation/dtos/MouseClickEvent.h
        src/infrastructure/opengl/GlfwInputAdapter.cpp
        src/infrastructure/opengl/GlfwInputAdapter.h
        src/application/use_cases/CreatePoint.cpp
        src/application/use_cases/CreatePoint.h
        src/domain/entities/Joint.cpp
        src/domain/entities/Joint.h
        src/application/dtos/Scene.h
        src/application/ports/ApplicationController.h
        src/infrastructure/opengl/OpenGlApplication.cpp
        src/infrastructure/opengl/OpenGlApplication.h
        src/application/use_cases/UpdateScene.cpp
        src/application/use_cases/UpdateScene.h
        src/application/use_cases/MoveHeadPoint.cpp
        src/application/use_cases/MoveHeadPoint.h
        src/domain/entities/Snake.cpp
        src/domain/entities/Snake.h
        src/application/use_cases/TransformPointsToSnake.cpp
        src/application/use_cases/TransformPointsToSnake.h
        src/domain/policies/AngleConstraintPolicy.cpp
        src/domain/policies/AngleConstraintPolicy.h
        src/application/use_cases/TranslateJointsUseCase.cpp
        src/application/use_cases/TranslateJointsUseCase.h
        src/domain/policies/DistanceConstraintPolicy.cpp
        src/domain/policies/DistanceConstraintPolicy.h
        src/domain/entities/Character.h
)

target_link_libraries(procedural_animation glad::glad glfw opengl::opengl glm Freetype::Freetype)

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/shaders")
foreach (shader object.shader.vs object.shader.fs text.shader.vs text.shader.fs)
    configure_file("src/infrastructure/opengl/shaders/${shader}" "shaders/${shader}" COPYONLY)
endforeach ()
target_compile_definitions(procedural_animation PRIVATE
        SHADER_DIRECTORY="${CMAKE_CURRENT_BINARY_DIR}/shaders")
