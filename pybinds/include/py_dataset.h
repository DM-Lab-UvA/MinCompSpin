#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "data/dataset.h"
#include "py_model.h"
#include "py_partition.h"

namespace py = pybind11;

class PyData {
public:
    /**
     * Constructs a new PyData object
     * 
     * @param file                  Path to the file.
     * @param n_var                 Number of variables in the system
     * @param n_states              Number of values each variable can take
     */
    PyData(const std::string& filename, int n_var, int n_states) : data(filename, n_var, n_states) {};

    double calc_log_ev_array(py::array_t<int8_t> partition);
    double calc_log_ev_model(PyModel& model);
    py::array calc_log_ev_icc(PyModel& model);
    py::array calc_log_ev_icc_array(py::array_t<int8_t> partition);
    
    double entropy(int base = -1);

    int get_n() {return this->data.n;};
    int get_N() {return this->data.N;};
    int get_N_unique() {return this->data.N_unique;};
    int get_q() {return this->data.q;};

    Data data;
};

void bind_data_class(py::module &);