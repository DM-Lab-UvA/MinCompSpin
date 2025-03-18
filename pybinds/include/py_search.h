#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "search/search.h"
#include "py_dataset.h"
#include "py_model.h"
#include "py_partition.h"

namespace py = pybind11;

class PyMCMSearch {
public:
    PyMCMSearch() : searcher() {};

    PyModel get_model_in();
    PyModel get_model_out();

    // Search methods
    PyModel exhaustive_search(PyData& pydata);
    PyModel greedy_search(PyData& pydata, PyModel* pymodel = nullptr);
    PyModel divide_and_conquer(PyData& pydata, PyModel* pymodel = nullptr);
    PyModel simulated_annealing(PyData& pydata, PyModel* pymodel = nullptr);

    // Setters and getters for the simulated annealing settings
    void set_SA_max_iter(int n_iter) {this->searcher.set_SA_max_iter(n_iter);};
    void set_SA_init_temp(int temp) {this->searcher.set_SA_init_temp(temp);};
    void set_SA_update_schedule(int n_iter) {this->searcher.set_SA_update_schedule(n_iter);};
    int get_SA_max_iter() {return this->searcher.get_SA_max_iter();};
    int get_SA_init_temp() {return this->searcher.get_SA_init_temp();};
    int get_SA_update_schedule() {return this->searcher.get_SA_update_schedule();};

    // Log-evidence trajectory
    py::array return_log_ev_trajectory() {return py::array(this->searcher.get_log_evidence_trajectory().size(), this->searcher.get_log_evidence_trajectory().data());};

    MCMSearch searcher;
};

void bind_search_mcm_class(py::module &);