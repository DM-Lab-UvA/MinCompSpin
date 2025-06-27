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
        .def(py::init<>())
        .def("get_mcm_in", &PyMCMSearch::get_mcm_in)
        .def("get_mcm_out", &PyMCMSearch::get_mcm_out)
        .def("exhaustive", &PyMCMSearch::exhaustive_search, py::arg("data"))
        .def("hierarchical_greedy_merging", &PyMCMSearch::greedy_search, py::arg("data"), py::arg("mcm_in") = nullptr, py::arg("filename") = "")
        .def("hierarchical_greedy_divisive", &PyMCMSearch::divide_and_conquer, py::arg("data"), py::arg("mcm_in") = nullptr, py::arg("filename") = "")
        .def("simulated_annealing", &PyMCMSearch::simulated_annealing, py::arg("data"), py::arg("mcm_in") = nullptr, py::arg("filename") = "")
        .def_property("SA_max_iteration", &PyMCMSearch::get_SA_max_iter, &PyMCMSearch::set_SA_max_iter)
        .def_property("SA_temperature_initial", &PyMCMSearch::get_SA_init_temp, &PyMCMSearch::set_SA_init_temp)
        .def_property("SA_temperature_iteration_update", &PyMCMSearch::get_SA_update_schedule, &PyMCMSearch::set_SA_update_schedule)
        .def_property_readonly("log_evidence_trajectory", &PyMCMSearch::return_log_ev_trajectory);
}