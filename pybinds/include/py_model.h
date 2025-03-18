#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "model/model.h"
#include "py_partition.h"

namespace py = pybind11;

class PyModel {
public:
    PyModel(int n) : model(n) {};
    PyModel(int n, py::array_t<int8_t> partition);
    PyModel(int n, std::string partition) : model(n, partition) {};

    py::array_t<int8_t> get_partition_array();
    void set_partition_array(py::array_t<int8_t> partition);

    py::array_t<int8_t> get_partition_as_gray_code();
    void set_partition_gray_code(py::array_t<int8_t> partition);

    void move_var_in(int var_index, int comp_index);
    void move_var_out(int var_index);
    void move_var(int var_index, int comp_index);

    double get_best_log_ev();
    py::array get_best_log_ev_per_icc();

    int get_n() {return this->model.n;};
    int get_n_comp() {return this->model.n_comp;};
    int get_rank() {return this->model.rank;};
    bool is_optimized() {return this->model.optimized;};
    
    Model model;
};

void bind_model_class(py::module &);