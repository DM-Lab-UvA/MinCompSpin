cmake_minimum_required(VERSION 3.26.4)

project(MinCompSpin)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
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
    find_package(Python COMPONENTS Interpreter Development)
    find_package(pybind11 CONFIG)

    set(python_bindings_sources)

    add_subdirectory(pybinds)

    pybind11_add_module(mcm_py MODULE ${python_bindings_sources})

    target_link_libraries(mcm_py PRIVATE ${PROJECT_NAME})

    set_target_properties(mcm_py PROPERTIES
                          LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/pybinds")
    
    target_include_directories(mcm_py PUBLIC
                          "${PROJECT_SOURCE_DIR}/pybinds/include"
                          "${PROJECT_SOURCE_DIR}/include"
                          )
endif()

add_library(${PROJECT_NAME} STATIC)

add_subdirectory(src)

target_include_directories(${PROJECT_NAME} PUBLIC
                          "${PROJECT_SOURCE_DIR}/include"
                          )