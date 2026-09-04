cmake_minimum_required(VERSION 3.16)

project(
    random
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(random
    src/main.cpp
    src/core/app.cpp
    src/core/utils.cpp
    src/args/options.cpp
    src/args/long_args.cpp
    src/args/short_args.cpp
    src/args/args.cpp
)

target_include_directories(random PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/)

target_compile_definitions(random PRIVATE
    APP_VERSION="${PROJECT_VERSION}"
    APP_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    APP_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    APP_VERSION_PATCH=${PROJECT_VERSION_PATCH}
)
target_compile_definitions(random PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
)
