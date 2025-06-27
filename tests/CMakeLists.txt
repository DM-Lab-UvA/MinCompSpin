add_subdirectory(googletest)

#basis/gauge_transform.cpp

add_executable(test_data data/dataset.cpp data/processing.cpp data/evidence.cpp)
target_link_libraries(test_data gtest_main ${PROJECT_NAME})
add_test(NAME test_data COMMAND test_data)
set_tests_properties(test_data PROPERTIES WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests)

add_executable(test_basis basis/basis.cpp basis/independence.cpp)
target_link_libraries(test_basis gtest_main ${PROJECT_NAME})
add_test(NAME test_basis COMMAND test_basis)
set_tests_properties(test_basis PROPERTIES WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests)

add_executable(test_model model/mcm.cpp)
target_link_libraries(test_model gtest_main ${PROJECT_NAME})
add_test(NAME test_model COMMAND test_model)
set_tests_properties(test_model PROPERTIES WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests)

add_executable(test_mcm_search search/mcm_search.cpp)
target_link_libraries(test_mcm_search gtest_main ${PROJECT_NAME})
add_test(NAME test_mcm_search COMMAND test_mcm_search)
set_tests_properties(test_mcm_search PROPERTIES WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests)

add_executable(test_basis_search search/basis_search.cpp)
target_link_libraries(test_basis_search gtest_main ${PROJECT_NAME})
add_test(NAME test_basis_search COMMAND test_basis_search)
set_tests_properties(test_basis_search PROPERTIES WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests)

add_executable(test_utilities utilities/histogram.cpp utilities/spin_ops.cpp)
target_link_libraries(test_utilities gtest_main ${PROJECT_NAME})
add_test(NAME test_utilities COMMAND test_utilities)
set_tests_properties(test_utilities PROPERTIES WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests)
