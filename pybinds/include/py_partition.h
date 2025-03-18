#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include <vector>
#include <iostream>

namespace py = pybind11;

py::array_t<int8_t> convert_partition_to_py(std::vector<__uint128_t>& partition, int n, int n_comp);
py::array_t<int8_t> convert_partition_to_py_gray_code(std::vector<__uint128_t>& partition, int n_comp);

std::vector<__uint128_t> convert_partition_from_py_2d_array(py::array_t<int8_t>& py_partition);
std::vector<__uint128_t> convert_partition_from_py_gray_code(py::array_t<int8_t>& py_partition, int n); 

__uint128_t convert_component_from_py(py::array_t<uint8_t>& component);