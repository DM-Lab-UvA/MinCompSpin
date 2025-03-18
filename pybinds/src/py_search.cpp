#include "py_search.h"

PyModel PyMCMSearch::get_model_in(){
    PyModel mcm(this->searcher.get_model_in().n);
    mcm.model = this->searcher.get_model_in();

    return mcm;
}

PyModel PyMCMSearch::get_model_out(){
    PyModel mcm(this->searcher.get_model_out().n);
    mcm.model = this->searcher.get_model_out();

    return mcm;
}

PyModel PyMCMSearch::exhaustive_search(PyData& pydata) {
    PyModel mcm(pydata.get_n());
    mcm.model = this->searcher.exhaustive_search(pydata.data);
    return mcm;
}

PyModel PyMCMSearch::greedy_search(PyData& pydata, PyModel* pymodel) {
    PyModel mcm(pydata.get_n());
    if (pymodel){
        mcm.model = this->searcher.greedy_search(pydata.data, &pymodel->model);
    }
    else{
        mcm.model = this->searcher.greedy_search(pydata.data);
    }
    return mcm;
}

PyModel PyMCMSearch::divide_and_conquer(PyData& pydata, PyModel* pymodel) {
    PyModel mcm(pydata.get_n());
    if (pymodel){
        mcm.model = this->searcher.divide_and_conquer(pydata.data, &pymodel->model);
    }
    else{
        mcm.model = this->searcher.divide_and_conquer(pydata.data);
    }
    return mcm;
}

PyModel PyMCMSearch::simulated_annealing(PyData& pydata, PyModel* pymodel) {
    PyModel mcm(pydata.get_n());
    if (pymodel){
        mcm.model = this->searcher.simulated_annealing(pydata.data, &pymodel->model);
    }
    else{
        mcm.model = this->searcher.simulated_annealing(pydata.data);
    }
    return mcm;
}

void bind_search_mcm_class(py::module &m) {
    py::class_<PyMCMSearch>(m, "MCMSearch")
        .def(py::init<>())
        .def("get_model_in", &PyMCMSearch::get_model_in)
        .def("get_model_out", &PyMCMSearch::get_model_out)
        .def("exhaustive_search", &PyMCMSearch::exhaustive_search, py::arg("Data"))
        .def("greedy_search", &PyMCMSearch::greedy_search, py::arg("Data"), py::arg("Model") = nullptr)
        .def("divide_and_conquer", &PyMCMSearch::divide_and_conquer, py::arg("Data"), py::arg("Model") = nullptr)
        .def("simulated_annealing", &PyMCMSearch::simulated_annealing, py::arg("Data"), py::arg("Model") = nullptr)
        .def_property("SA_max_iterations", &PyMCMSearch::get_SA_max_iter, &PyMCMSearch::set_SA_max_iter)
        .def_property("SA_init_temperature", &PyMCMSearch::get_SA_init_temp, &PyMCMSearch::set_SA_init_temp)
        .def_property("SA_update_schedule", &PyMCMSearch::get_SA_update_schedule, &PyMCMSearch::set_SA_update_schedule)
        .def_property_readonly("log_evidence_trajectory", &PyMCMSearch::return_log_ev_trajectory);
}