add_subdirectory(googletest)

add_executable(testing 
              data/dataset.cpp
              data/processing.cpp
              data/evidence.cpp
              basis/independence.cpp
              basis/basis.cpp
              #basis/gauge_transform.cpp
              model/mcm.cpp
              search/mcm_search.cpp
              search/basis_search.cpp
              utilities/histogram.cpp
              utilities/spin_ops.cpp)

target_link_libraries(testing gtest_main ${PROJECT_NAME})

add_test(NAME testing COMMAND testing)