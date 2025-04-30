list(APPEND python_bindings_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/py_dataset.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/py_mcm.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/py_basis.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/py_mcm_search.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/py_basis_search.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/py_partition.cpp)

set(python_bindings_sources ${python_bindings_sources} PARENT_SCOPE)