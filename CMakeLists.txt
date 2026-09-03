cmake_minimum_required(VERSION 3.16)

project(
    random
    VERSION 0.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(random
    src/main.cpp
)

target_include_directories(random PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/)
