#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "model/mcm.h"
#include "py_partition.h"

namespace py = pybind11;

// Forward declaration
class PyData;

class PyMCM {
public:
    PyMCM(int n) : mcm(n) {};
    PyMCM(int n, py::array_t<int8_t> partition);
    PyMCM(int n, std::string partition) : mcm(n, partition) {};

    py::array_t<int8_t> get_partition_array();
    void set_partition_array(py::array_t<int8_t> partition);

    py::array_t<int8_t> get_partition_as_gray_code();
    void set_partition_gray_code(py::array_t<int8_t> partition);

    void move_var_in(int var_index, int comp_index);
    void move_var_out(int var_index);
    void move_var(int var_index, int comp_index);

    double get_best_log_ev();
    py::array get_best_log_ev_per_icc();
    void print_info() {return this->mcm.print_info();};

    void generate_samples(int N, PyData& pydata, std::string file_name);
    PyData generate_data(int N, PyData& pydata, std::string file_name);

    int get_n() {return this->mcm.n;};
    int get_n_comp() {return this->mcm.n_comp;};
    int get_rank() {return this->mcm.rank;};
    bool is_optimized() {return this->mcm.optimized;};
    
    MCM mcm;
};

void bind_mcm_class(py::module &);