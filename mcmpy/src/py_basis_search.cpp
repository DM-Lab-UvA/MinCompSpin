#include "py_basis_search.h"

PyBasis PyBasisSearch::up_to_order_k(PyData& pydata, int k, std::string file_name){
    PyBasis pybasis(pydata.get_n(), pydata.get_q());
    pybasis.basis = this->searcher.up_to_order_k(pydata.data, k, file_name);

    return pybasis;
}

PyBasis PyBasisSearch::iterative_search(PyData& pydata, int k, std::string file_name){
    PyBasis pybasis(pydata.get_n(), pydata.get_q());
    pybasis.basis = this->searcher.iterative_search(pydata.data, k, file_name);

    return pybasis;
}

PyBasis PyBasisSearch::get_basis() {
    PyBasis pybasis(1,2);
    pybasis.basis = this->searcher.get_basis();

    return pybasis;
}

void bind_search_basis_class(py::module &m) {
    py::class_<PyBasisSearch>(m, "BasisSearch")
        .def(py::init<>())
        .def("exhaustive", &PyBasisSearch::exhaustive_search, py::arg("data"), py::arg("filename") = "")
        .def("up_to_order_k_iterative", &PyBasisSearch::iterative_search, py::arg("data"), py::arg("k"), py::arg("filename") = "")
        .def("up_to_order_k_fixed", &PyBasisSearch::up_to_order_k, py::arg("data"), py::arg("k"), py::arg("filename") = "")
        .def("get_basis", &PyBasisSearch::get_basis)
        .def("print_result", &PyBasisSearch::print_search_results);
}