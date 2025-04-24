cmake_minimum_required(VERSION 3.30)

project(week4)

find_package(SDL3 CONFIG REQUIRED)
find_package(flecs CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_sources(${PROJECT_NAME} PRIVATE
    Node.hpp
    Texture.hpp
    Model.hpp Model.cpp
    Camera.hpp Camera.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3
    flecs::flecs
    glm::glm)
