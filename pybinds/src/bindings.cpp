#include "py_dataset.h"
#include "py_mcm.h"
#include "py_search.h"
#include "py_basis.h"

PYBIND11_MODULE(mcm_py, m) {
    bind_data_class(m);
    bind_mcm_class(m);
    bind_basis_class(m);
    bind_search_mcm_class(m);
}