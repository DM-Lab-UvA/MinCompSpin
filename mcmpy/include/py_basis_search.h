#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "search/basis_search/basis_search.h"
#include "py_dataset.h"
#include "py_basis.h"

namespace py = pybind11;

class PyBasisSearch {
public:
    PyBasisSearch() : searcher() {};

    // Search methods
    PyBasis up_to_order_k(PyData& pydata, int k, std::string file_name = "");
    PyBasis iterative_search(PyData& pydata, int k, std::string file_name = "");
    PyBasis exhaustive_search(PyData& pydata, std::string file_name = "") {return this->up_to_order_k(pydata, pydata.get_n(), file_name);};

    PyBasis get_basis();
    void print_search_results() {searcher.print_search_results();};

    BasisSearch searcher;
};

void bind_search_basis_class(py::module &m);