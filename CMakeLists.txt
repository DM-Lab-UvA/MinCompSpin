cmake_minimum_required(VERSION 3.26.4)

project(MinCompSpin)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# Optimization flag
set(CMAKE_CXX_FLAGS "-O3")

# Tests
option(BUILD_TESTS "Build tests" ON)
# Pybindings
option(BUILD_PYBIND "Build Pybindings" ON)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(BUILD_PYBIND)
    add_subdirectory(mcmpy)
endif()

add_library(${PROJECT_NAME} STATIC)

add_subdirectory(src)

target_include_directories(${PROJECT_NAME} PUBLIC
                          "${PROJECT_SOURCE_DIR}/include"
                          )
                          