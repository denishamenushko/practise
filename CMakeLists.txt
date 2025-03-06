cmake_minimum_required(VERSION 3.16)

project(test)

find_package(SDL3 CONFIG REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_sources(${PROJECT_NAME} PRIVATE model.hpp model.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

