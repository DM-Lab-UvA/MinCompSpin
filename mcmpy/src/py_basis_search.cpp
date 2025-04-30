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
        .def("exhaustive", &PyBasisSearch::exhaustive_search, py::arg("data"), py::arg("filename") = "",
        R"pbdoc(
            Performs a search over all spin operators to find the optimal basis for a given dataset.

            This function searches over all spin operators of the variables and selects the optimal set of `n` linearly independent spin operators with the lowest entropy over the data.
            The result is returned as an `Basis` object and stored in the internal variable `basis`.
            **Note** that this search is only feasible for small systems (n < 15).

            Parameters
            ----------
            data : Data
                The dataset for which the optimal MCM will be determined.
            filename : string, optional
                Path to the file where the basis will be written.
                If not provided, nothing will be written to a file.
            
            Returns
            -------
            Basis
                The optimal basis for the given dataset found by an exhaustive search.
            )pbdoc")
        .def("up_to_order_k_iterative", &PyBasisSearch::iterative_search, py::arg("data"), py::arg("k"), py::arg("filename") = "",
        R"pbdoc(
            Performs an iterative search over all spin operators up to order k to find the optimal basis for a given dataset.

            This function searches iteratively over all spin operators up to order k and selects the set of `n` linearly independent spin operators with the lowest entropy over the data.
            In each iteration the dataset is transformed in the current best basis, and a new optimal basis is searched for.
            This process continues until the optimal basis between two iterations remains unchanged.
            The result is returned as an `Basis` object and stored in the internal variable `basis`.

            Parameters
            ----------
            data : Data
                The dataset for which the optimal MCM will be determined.
            k : int
                The maximum interaction order of the spin operators to consider in each iteration.
            filename : string, optional
                Path to the file where the basis will be written.
                If not provided, nothing will be written to a file.
            
            Returns
            -------
            Basis
                The optimal basis for the given dataset found by an iterative search up to order k.
            )pbdoc")
        .def("up_to_order_k_fixed", &PyBasisSearch::up_to_order_k, py::arg("data"), py::arg("k"), py::arg("filename") = "",
        R"pbdoc(
            Performs a search over all spin operators up to order k to find the optimal basis for a given dataset.

            This function searches over all spin operators up to order k and selects the set of `n` linearly independent spin operators with the lowest entropy over the data.
            The result is returned as an `Basis` object and stored in the internal variable `basis`.

            Parameters
            ----------
            data : Data
                The dataset for which the optimal MCM will be determined.
            k : int
                The maximum interaction order of the spin operators to consider.
            filename : string, optional
                Path to the file where the basis will be written.
                If not provided, nothing will be written to a file.
            
            Returns
            -------
            Basis
                The optimal basis for the given dataset found by a search up to order k.
            )pbdoc")
        .def("get_basis", &PyBasisSearch::get_basis,
            R"pbdoc(
            Returns a Basis object containing the solution of the last search.

            **Note** that this function can only be called once a search has been run.

            Returns
            -------
            Basis
                The Basis object that was the result of the last search.
            )pbdoc")
        .def("print_results", &PyBasisSearch::print_search_results,
            R"pbdoc(
            Prints out the results of the last search.

            This function prints the search type, duration and the basis found by the last search.
            For a search up to order k, the maximum interaction order is also printed out.
            For an iterative search, the maximum interaction order and the number of iterations is also printed out.
            )pbdoc");
}