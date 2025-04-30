#include "py_dataset.h"
#include "py_mcm.h"
#include "py_mcm_search.h"
#include "py_basis.h"
#include "py_basis_search.h"

PYBIND11_MODULE(mcmpy, m) {
    bind_data_class(m);
    bind_mcm_class(m);
    bind_basis_class(m);
    bind_search_mcm_class(m);
    bind_search_basis_class(m);
}