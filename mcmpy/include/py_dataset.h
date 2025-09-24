#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "data/dataset.h"
#include "utilities/spin_ops.h"
#include "utilities/miscellaneous.h"
#include "py_mcm.h"
#include "py_partition.h"

namespace py = pybind11;

std::vector<__uint128_t> convert_spin_op_from_py(const py::array_t<uint8_t>& spin_op, int q, int n_ints);

class PyData {
public:
    /**
     * Constructs a new PyData object from a file
     * 
     * @param file                  Path to the file.
     * @param n_var                 Number of variables in the system
     * @param n_states              Number of values each variable can take
     */
    PyData(const std::string& filename, int n_var, int n_states) : data(filename, n_var, n_states) {};

    /**
     * Constructs a new PyData object from a numpy array
     * 
     * @param array                 Numpy array
     * @param n_var                 Number of variables in the system
     * @param n_states              Number of values each variable can take
     */
    PyData(py::array_t<int> array, int n_var, int n_states);

    /**
     * Constructs a new PyData object from a Data object
     * 
     * @param _data                 Data object
     */
    PyData(const Data& _data) : data(_data.dataset, _data.n, _data.q, _data.N) {};

    int processing_numpy(py::array_t<int> input_array,
                                int n_var,
                                int n_ints,
                                int n_states,
                                std::vector<std::pair<std::vector<__uint128_t>, unsigned int>>& data);

    double calc_property_array(py::array_t<int8_t> partition, std::string property);
    double calc_property_mcm(PyMCM& mcm, std::string property);
    py::array calc_property_icc_array(py::array_t<int8_t> partition, std::string property);
    py::array calc_property_icc(PyMCM& mcm, std::string property);

    double calc_log_ev_array(py::array_t<int8_t> partition) {return this->calc_property_array(partition, "evidence");};
    double calc_log_ev_mcm(PyMCM& mcm) {return this->calc_property_mcm(mcm, "evidence");};
    py::array calc_log_ev_icc_array(py::array_t<int8_t> partition) {return this->calc_property_icc_array(partition, "evidence");};
    py::array calc_log_ev_icc(PyMCM& mcm) {return this->calc_property_icc(mcm, "evidence");};

    double calc_log_likelihood_array(py::array_t<int8_t> partition) {return this->calc_property_array(partition, "likelihood");};
    double calc_log_likelihood_mcm(PyMCM& mcm) {return this->calc_property_mcm(mcm, "likelihood");};
    py::array calc_log_likelihood_icc_array(py::array_t<int8_t> partition) {return this->calc_property_icc_array(partition, "likelihood");};
    py::array calc_log_likelihood_icc(PyMCM& mcm) {return this->calc_property_icc(mcm, "likelihood");};

    double calc_geom_complexity_array(py::array_t<int8_t> partition) {return this->calc_property_array(partition, "geom_compl");};
    double calc_geom_complexity_mcm(PyMCM& mcm) {return this->calc_property_mcm(mcm, "geom_compl");};
    py::array calc_geom_complexity_icc_array(py::array_t<int8_t> partition) {return this->calc_property_icc_array(partition, "geom_compl");};
    py::array calc_geom_complexity_icc(PyMCM& mcm) {return this->calc_property_icc(mcm, "geom_compl");};

    double calc_param_complexity_array(py::array_t<int8_t> partition) {return this->calc_property_array(partition, "param_compl");};
    double calc_param_complexity_mcm(PyMCM& mcm) {return this->calc_property_mcm(mcm, "param_compl");};
    py::array calc_param_complexity_icc_array(py::array_t<int8_t> partition) {return this->calc_property_icc_array(partition, "param_compl");};
    py::array calc_param_complexity_icc(PyMCM& mcm) {return this->calc_property_icc(mcm, "param_compl");};

    double calc_mdl_array(py::array_t<int8_t> partition) {return this->calc_property_array(partition, "mdl");};
    double calc_mdl_mcm(PyMCM& mcm) {return this->calc_property_mcm(mcm, "mdl");};
    
    double entropy(int base = -1);
    double entropy_of_spin_op(const py::array_t<int8_t>& op);

    int get_n() {return this->data.n;};
    int get_N() {return this->data.N;};
    int get_N_unique() {return this->data.N_unique;};
    int get_q() {return this->data.q;};

    Data data;
};

void bind_data_class(py::module &);
