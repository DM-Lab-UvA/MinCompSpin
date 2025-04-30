#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "py_dataset.h"
#include "basis/basis.h"

namespace py = pybind11;

py::array_t<uint8_t> convert_basis_to_py(const std::vector<std::vector<uint8_t>>& spin_ops, int n);
std::vector<std::vector<uint8_t>> convert_basis_from_py(const py::array_t<uint8_t>& spin_ops);

class PyBasis {
public:
    PyBasis(int n, int q) : basis(n, q) {};
    PyBasis(int n, int q, py::array_t<uint8_t>& spin_ops) : basis(n, q, convert_basis_from_py(spin_ops)) {};
    PyBasis(int n, int q, std::string& filename) : basis(n, q, filename) {};

    py::array_t<uint8_t> get_basis() {return convert_basis_to_py(this->basis.get_basis_matrix(), this->basis.get_n());};
    void set_basis(py::array_t<uint8_t>& spin_ops) {this->basis.set_basis(convert_basis_from_py(spin_ops));};
    void set_basis_from_file(std::string& filename) {this->basis.set_basis_from_file(filename);};
    void set_basis_default() {return this->basis.set_basis_default();};
    int set_basis_random() {return this->basis.set_basis_random();};

    void gt_data_in_place(PyData& dataset) {this->basis.gt_data_in_place(dataset.data);};
    PyData gt_data(PyData& dataset);

    int get_n() {return basis.get_n();};
    int get_q() {return basis.get_q();};

    void print_details() {basis.print_details();};

    Basis basis;
};

void bind_basis_class(py::module &);