#include "py_basis.h"

py::array_t<uint8_t> convert_basis_to_py(const std::vector<std::vector<uint8_t>>& spin_ops, int n){
    // Initialize 2d python array
    py::array_t<uint8_t> conv_basis = py::array_t<uint8_t>({n, n});

    py::buffer_info buff = conv_basis.request();
    uint8_t *ptr = static_cast<uint8_t *>(buff.ptr);

    std::vector<uint8_t> spin_op;
    for (int i = 0; i < n; ++i){
        spin_op = spin_ops[i];
        for (int j = 0; j < n; ++j){
            ptr[i*n + j] = spin_op[j];
        }
    }
    return conv_basis;
}

std::vector<std::vector<uint8_t>> convert_basis_from_py(const py::array_t<uint8_t>& spin_ops){
    py::buffer_info buff = spin_ops.request();

    // Check the dimensions
    int ndim = buff.ndim;
    if (ndim != 2){
        throw std::invalid_argument("The basis should be given as a 2 dimensional array.");
    }
    // Check if the system size is valid
    int n = buff.shape[0];
    if (n > 128){
        throw std::invalid_argument("The maximum system size is 128.");
    }
    if (buff.shape[0] != buff.shape[1]){
        throw std::invalid_argument("The basis should be given as an n by n matrix.");
    }

    std::vector<std::vector<uint8_t>> conv_basis(n, std::vector<uint8_t>(n));
    uint8_t *ptr = static_cast<uint8_t *>(buff.ptr);

    for (int i = 0; i < n; ++i){
        for (int j = 0; j < n; ++j){
            conv_basis[i][j] = ptr[i * n + j];
        }
    }
    return conv_basis;
}

PyData PyBasis::gt_data(PyData& dataset){
    // Copy the given dataset
    PyData gt_data = dataset;
    // Transform the copy in place
    this->gt_data_in_place(gt_data);
    return gt_data;
}

void bind_basis_class(py::module &m) {
    py::class_<PyBasis>(m, "Basis")
        .def(py::init<int, int>())
        .def(py::init<int, int, py::array_t<uint8_t>&>())
        .def(py::init<int, int, std::string&>())
        .def("set_from_file", &PyBasis::set_basis_from_file, py::arg("filename"))
        .def("set_default", &PyBasis::set_basis_default)
        .def("set_random", &PyBasis::set_basis_random)
        .def("gauge_transform_data", &PyBasis::gt_data, py::arg("data"))
        .def("gauge_transform_data_in_place", &PyBasis::gt_data_in_place, py::arg("data"))
        .def("print_details", &PyBasis::print_details)
        .def_property("matrix", &PyBasis::get_basis, &PyBasis::set_basis)
        .def_property_readonly("n", &PyBasis::get_n)
        .def_property_readonly("q", &PyBasis::get_q);
}