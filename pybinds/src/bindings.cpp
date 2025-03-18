#include "../include/py_dataset.h"
#include "../include/py_model.h"
#include "../include/py_search.h"

PYBIND11_MODULE(mcm_py, m) {
    bind_data_class(m);
    bind_model_class(m);
    bind_search_mcm_class(m);
}