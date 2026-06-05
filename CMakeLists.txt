cmake_minimum_required(VERSION 3.16)

project(random)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SRC "src/*.cpp")

add_executable(random ${SRC})
