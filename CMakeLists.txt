cmake_minimum_required(VERSION 3.20)

project(fengling_trainer
    VERSION 0.1.0
    DESCRIPTION "A Windows game trainer project skeleton"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(fengling_trainer
    src/main.cpp
    src/core/cheat.cpp
    src/core/config.cpp
    src/core/memory.cpp
    src/core/trainer.cpp
    src/platform/process.cpp
)

target_include_directories(fengling_trainer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if (WIN32)
    target_compile_definitions(fengling_trainer PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN)
endif()
