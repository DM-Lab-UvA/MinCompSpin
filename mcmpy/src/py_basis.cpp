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
        .def("set_from_file", &PyBasis::set_basis_from_file, py::arg("filename"),
        R"pbdoc(
        Updates the basis using a given file.

        If the given file contains a valid basis, then the object will update its internal basis variable.
        The file should contain n lines, each representing an operator.
        On each line, the first n characters should be values between 0 and q-1 which indicates the power with which the corresponding variable is present in the operator.
        For the basis to be valid, the given set of operators should also be linearly independent.

        Parameters
        ----------
        filename : string
            Path to the file where the basis is written.
        )pbdoc")
        .def("set_default", &PyBasis::set_basis_default,
        R"pbdoc(
        Resets the basis to the default one.

        This function will set the internal basis variable to the default basis.
        The default basis corresponds to the n field operators on each variable.
        )pbdoc")
        .def("set_random", &PyBasis::set_basis_random, 
        R"pbdoc(
        Sets the basis to a valid random one.

        This function will generate random n by n matrices with entries between 0 and q-1 until its columns form a linearly independent basis.

        Returns
        ------
        int
            The number of matrices generated before finding a linearly independent one.
        )pbdoc")
        .def("gauge_transform_data", &PyBasis::gt_data, py::arg("data"),
        R"pbdoc(
        Applies a gauge transformation to the given dataset.

        This function takes a data object expressed in the variables **s** and transforms it
        such that it is expressed in the variables **σ**, where **σ** are the spin operators that make up the internal basis.
        The input data object is not modified.

        Parameters
        ----------
        data : Data
            The dataset expressed in the variables **s** that will be transformed.
        
        Returns
        -------
        Data
            A Data object containing the dataset expressed in the variables **σ**.
        )pbdoc")
        .def("gauge_transform_data_in_place", &PyBasis::gt_data_in_place, py::arg("data"),
        R"pbdoc(
        Applies a gauge transformation to the given dataset in place.

        This function takes a data object expressed in the variables **s** and transforms it
        such that it is expressed in the variables **σ**, where **σ** are the spin operators that make up the internal basis.
        The input data object is modified and will contain the transformed dataset.

        Parameters
        ----------
        data : Data
            The dataset expressed in the variables **s** that will be transformed.
        
        )pbdoc")
        .def("print_details", &PyBasis::print_details,
        R"pbdoc(
        Prints the basis to the terminal.

        Every line corresponds to an operator in the basis.
        Operators are displayed as a string of n characters with values between 0 and q-1 which indicates the power with which the corresponding variable is present in the operator.
        On each line, after the operator string, the indices of the variables that are included in the spin operator are also printed.
        )pbdoc")
        .def_property("matrix", &PyBasis::get_basis, &PyBasis::set_basis, "The matrix representation of basis, with shape (n, n). Each column represents a spin operator."
        "Each element of a column is a value between 0 and q-1 indicating the power with which the corresponding variable is present in the operator.")
        .def_property_readonly("n", &PyBasis::get_n, "The number of variables in the system (read-only).")
        .def_property_readonly("q", &PyBasis::get_q, "The number of states each variable can take (read-only).");
}