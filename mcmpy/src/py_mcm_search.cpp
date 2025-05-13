#include "py_mcm_search.h"

PyMCM PyMCMSearch::get_mcm_in(){
    PyMCM mcm(this->searcher.get_mcm_in().n);
    mcm.mcm = this->searcher.get_mcm_in();

    return mcm;
}

PyMCM PyMCMSearch::get_mcm_out(){
    PyMCM mcm(this->searcher.get_mcm_out().n);
    mcm.mcm = this->searcher.get_mcm_out();

    return mcm;
}

PyMCM PyMCMSearch::exhaustive_search(PyData& pydata) {
    PyMCM mcm(pydata.get_n());
    mcm.mcm = this->searcher.exhaustive_search(pydata.data);
    return mcm;
}

PyMCM PyMCMSearch::greedy_search(PyData& pydata, PyMCM* pymcm, std::string file_name) {
    PyMCM mcm(pydata.get_n());
    if (pymcm){
        mcm.mcm = this->searcher.greedy_search(pydata.data, &pymcm->mcm, file_name);
    }
    else{
        mcm.mcm = this->searcher.greedy_search(pydata.data, nullptr, file_name);
    }
    return mcm;
}

PyMCM PyMCMSearch::divide_and_conquer(PyData& pydata, PyMCM* pymcm, std::string file_name) {
    PyMCM mcm(pydata.get_n());
    if (pymcm){
        mcm.mcm = this->searcher.divide_and_conquer(pydata.data, &pymcm->mcm, file_name);
    }
    else{
        mcm.mcm = this->searcher.divide_and_conquer(pydata.data, nullptr, file_name);
    }
    return mcm;
}

PyMCM PyMCMSearch::simulated_annealing(PyData& pydata, PyMCM* pymcm, std::string file_name) {
    PyMCM mcm(pydata.get_n());
    if (pymcm){
        mcm.mcm = this->searcher.simulated_annealing(pydata.data, &pymcm->mcm);
    }
    else{
        mcm.mcm = this->searcher.simulated_annealing(pydata.data, nullptr, file_name);
    }
    return mcm;
}

void bind_search_mcm_class(py::module &m) {
    py::class_<PyMCMSearch>(m, "MCMSearch")
        .def(py::init<>(), 
        R"pbdoc(
        Constructs an MCMSearch object that contains several methods to find the best partition.
        )pbdoc")
        .def("get_mcm_in", &PyMCMSearch::get_mcm_in,
            R"pbdoc(
            Returns an MCM object containing the starting partition of the last search.

            **Note** that this function can only be called once a search has been run and if the search was not an exhaustive.

            Returns
            -------
            MCM
                The MCM that was used as starting point of the last search.
            )pbdoc")
        .def("get_mcm_out", &PyMCMSearch::get_mcm_out,
            R"pbdoc(
                Returns an MCM object containing the solution of the last search.
    
                **Note** that this function can only be called once a search has been run.
    
                Returns
                -------
                MCM
                    The MCM that was the result of the last search.
                )pbdoc")
        .def("exhaustive", &PyMCMSearch::exhaustive_search, py::arg("data"),
            R"pbdoc(
            Performs an exhaustive search to find the optimal MCM for a given dataset.

            This function searches over all possible partitions of the variables to find the MCM with the largest log-evidence for the provided dataset.
            The result is returned as an `MCM` object and stored in the internal variable `mcm_out`.
            **Note** that this search is only feasible for small systems (n < 15).

            Parameters
            ----------
            data : Data
                The dataset for which the optimal MCM will be determined.
            
            Returns
            -------
            MCM
                The MCM that has the largest log-evidence for the given dataset.
            )pbdoc")
        .def("hierarchical_greedy_merging", &PyMCMSearch::greedy_search, py::arg("data"), py::arg("mcm_in") = nullptr, py::arg("filename") = "",
            R"pbdoc(
            Performs a hierarchical greedy merging procedure to find the optimal MCM for a given dataset.
            
            This search function consists of an iterative merging procedure.
            Because the algorithm doesn't go through all partitions, there is no guarentee of finding the MCM with the largest log-evidence.
            The result is returned as an `MCM` object and stored in the internal variable `mcm_out`.

            Parameters
            ----------
            data : Data
                The dataset for which the optimal MCM will be determined.
            mcm_in : MCM, optional
                An optional MCM object representing the starting partition of the algorithm.
                If not provided, the independent model is used as the default starting partition.
            filename : string, optional
                Path to the file where the search details will be written.
                If not provided, nothing will be written to a file.

            Returns
            -------
            MCM
                The best fitting MCM for the given dataset found by the greedy merging algorithm.
            )pbdoc")
        .def("hierarchical_greedy_divisive", &PyMCMSearch::divide_and_conquer, py::arg("data"), py::arg("mcm_in") = nullptr, py::arg("filename") = "",
            R"pbdoc(
            Performs a hierarchical greedy divisive procedure to find the optimal MCM for a given dataset.

            This search function consists of an iterative division procedure.
            Because the algorithm doesn't go through all partitions, there is no guarentee of finding the MCM with the largest log-evidence.
            The result is returned as an `MCM` object and stored in the internal variable `mcm_out`.

            Parameters
            ----------
            data : Data
                The dataset for which the optimal MCM will be determined.
            mcm_in : MCM, optional
                An optional MCM object representing the starting partition of the algorithm.
                If not provided, the complete model is used as the default starting partition.
            filename : string, optional
                Path to the file where the search details will be written.
                If not provided, nothing will be written to a file.

            Returns
            -------
            MCM
                The best fitting MCM for the given dataset found by the greedy divisive algorithm.
            )pbdoc")
        .def("simulated_annealing", &PyMCMSearch::simulated_annealing, py::arg("data"), py::arg("mcm_in") = nullptr, py::arg("filename") = "",
        R"pbdoc(
            Performs a simulated annealing algorithm to find the optimal MCM for a given dataset.

            Because the algorithm doesn't go through all partitions, there is no guarentee of finding the MCM with the largest log-evidence.
            The result is returned as an `MCM` object and stored in the internal variable `mcm_out`.

            Parameters
            ----------
            data : Data
                The dataset for which the optimal MCM will be determined.
            mcm_in : MCM, optional
                An optional MCM object representing the starting partition of the algorithm.
                If not provided, the independent model is used as the default starting partition.
            filename : string, optional
                Path to the file where the search details will be written.
                If not provided, nothing will be written to a file.

            Returns
            -------
            MCM
                The best fitting MCM for the given dataset found by the simulated annealing algorithm.
            )pbdoc")
        .def_property("SA_max_iteration", &PyMCMSearch::get_SA_max_iter, &PyMCMSearch::set_SA_max_iter, "The maximum number of iterations in the simulated annealing algorithm. The default number of iterations is 50 000.")
        .def_property("SA_temperature_initial", &PyMCMSearch::get_SA_init_temp, &PyMCMSearch::set_SA_init_temp, "The initial temperature used in the annealing process. The default temperature is 100.")
        .def_property("SA_temperature_iteration_update", &PyMCMSearch::get_SA_update_schedule, &PyMCMSearch::set_SA_update_schedule, "The number of iterations after which the temperature is updated. The default number of iterations is 100.")
        .def_property_readonly("log_evidence_trajectory", &PyMCMSearch::return_log_ev_trajectory, "Array with the log-evidence values (read-only)."
            "For the simulated annealing, the log-evidence value of the current best solution at every iteration is stored. For the exhaustive search, the log-evidence of all possible partitions is stored."
            "For the hierarchical greedy algorithms, the log-evidence is stored only when an improvement is found.");
}