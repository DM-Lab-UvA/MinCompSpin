add_subdirectory(pybind11)

set(python_bindings_sources)

add_subdirectory(src)

pybind11_add_module(mcmpy MODULE ${python_bindings_sources})

target_include_directories(mcmpy PUBLIC
                        "${PROJECT_SOURCE_DIR}/mcmpy/include"
                        "${PROJECT_SOURCE_DIR}/include"
                        )

target_link_libraries(mcmpy PRIVATE ${PROJECT_NAME})

set_target_properties(mcmpy PROPERTIES
                      SUFFIX ".so"
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/mcmpy")
                
configure_file(__init__.py.in ${CMAKE_BINARY_DIR}/mcmpy/__init__.py)
configure_file(setup.py.in ${CMAKE_BINARY_DIR}/mcmpy/setup.py)
