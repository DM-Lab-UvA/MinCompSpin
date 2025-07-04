#include "py_mcm.h"
#include "py_dataset.h"

PyMCM::PyMCM(int n, py::array_t<int8_t> partition) : mcm(n) {
    std::vector<__uint128_t> conv_partition;

    // Check the dimensions of the array
    py::buffer_info buff = partition.request();
    int ndim = buff.ndim;

    if (ndim == 1){
        conv_partition = convert_partition_from_py_gray_code(partition, n);
    }
    else if (ndim == 2){
        conv_partition = convert_partition_from_py_2d_array(partition);
    }
    else{
        throw std::invalid_argument("The partition should be a 1D or 2D array.");
    }
    this->mcm.set_starting_partition(conv_partition);
}

py::array_t<int8_t> PyMCM::get_partition_array() {
    int n = this->mcm.n;
    int n_comp = this->mcm.n_comp;
    return convert_partition_to_py(this->mcm.get_partition(), n, n_comp);
}

void PyMCM::set_partition_array(py::array_t<int8_t> partition) {
    std::vector<__uint128_t> conv_partition = convert_partition_from_py_2d_array(partition);
    this->mcm.set_starting_partition(conv_partition);
}

py::array_t<int8_t> PyMCM::get_partition_as_gray_code() {
    int n_comp = this->mcm.n_comp;
    return convert_partition_to_py_gray_code(this->mcm.get_partition(), n_comp);
}

void PyMCM::set_partition_gray_code(py::array_t<int8_t> partition) {
    std::vector<__uint128_t> conv_partition = convert_partition_from_py_gray_code(partition, this->mcm.n);
    this->mcm.set_starting_partition(conv_partition);
}

void PyMCM::move_var_in(int var_index, int comp_index){
    // Check if the variable index is valid
    if (var_index >= this->get_n()){
        throw std::invalid_argument("The variable index should be between 0 and n-1.");
    }
    int n_comp = this->get_n_comp();
    // Check if the component index is valid
    if (comp_index > n_comp){
        // Set to the first empty component
        comp_index = n_comp;
    }
    __uint128_t var = 1 << var_index;
    // Check if the variable is not present yet
    for (int i = 0; i < n_comp; i++){
        if (var & this->mcm.get_partition()[i]){
            throw std::invalid_argument("This variable is already present in the partition.");
        }
    }
    // Add the variable
    if (!this->mcm.partition[comp_index]){
        // Moved to a new component
        this->mcm.n_comp++;
    }
    this->mcm.partition[comp_index] += var;
    this->mcm.rank++;

    // Reset if a search has been ran before
    if (this->mcm.optimized){
        this->mcm.optimized = false;
        this->mcm.log_ev_per_icc.clear();
    } 
}

void PyMCM::move_var_out(int var_index){
    // Check if the variable index is valid
    if (var_index >= this->get_n()){
        throw std::invalid_argument("The variable index should be between 0 and n-1.");
    }
    __uint128_t var = 1 << var_index;
    int n_comp = this->get_n_comp();
    int i = 0;
    // Find the component that contains this variable
    while(! (var & this->mcm.get_partition()[i])){
        if (i == n_comp-1){
            throw std::invalid_argument("This variable is not present in the partition.");
        }
        i++;
    }
    // Remove the variable
    this->mcm.partition[i] -= var;
    this->mcm.rank--;

    // Reduce the number of components
    if (! this->mcm.partition[i]){
        this->mcm.n_comp--;
        place_empty_entries_last(this->mcm.partition);
    }
    // Reset if a search has been ran before
    if (this->mcm.optimized){
        this->mcm.optimized = false;
        this->mcm.log_ev_per_icc.clear();
    } 
}

void PyMCM::move_var(int var_index, int comp_index){
    this->move_var_out(var_index);
    this->move_var_in(var_index, comp_index);

    // Reset if a search has been ran before
    if (this->mcm.optimized){
        this->mcm.optimized = false;
        this->mcm.log_ev_per_icc.clear();
    }
}

double PyMCM::get_best_log_ev() {
    return this->mcm.get_best_log_ev();
}

py::array PyMCM::get_best_log_ev_per_icc() {
    std::vector<double> ev_per_icc = this->mcm.get_best_log_ev_per_icc();
    return py::array(this->mcm.n_comp, ev_per_icc.data());
}

void PyMCM::generate_data_file(int N, PyData& pydata, std::string filename) {this->mcm.generate_data_file(N, pydata.data, filename);}

PyData PyMCM::generate_data_object(int N, PyData& pydata){
    Data samples = this->mcm.generate_data_object(N, pydata.data);
    PyData pysamples = PyData(samples);

    return pysamples;
}

void bind_mcm_class(py::module &m) {
    py::class_<PyMCM>(m, "MCM")
        .def(py::init<int>(), py::arg("n"))
        .def(py::init<int, py::array_t<int8_t>>(), py::arg("n"), py::arg("partition"))
        .def(py::init<int, const std::string&>(), py::arg("n"), py::arg("partition"))
        .def("move_variable_in", &PyMCM::move_var_in, py::arg("var_index"), py::arg("comp_index"))
        .def("move_variable_out", &PyMCM::move_var_out, py::arg("var_index"))
        .def("move_variable", &PyMCM::move_var, py::arg("var_index"), py::arg("comp_index"))
        .def("get_best_log_evidence", &PyMCM::get_best_log_ev)
        .def("get_best_log_evidence_icc", &PyMCM::get_best_log_ev_per_icc)
        .def("print_details", &PyMCM::print_info)
        .def("generate_data_file", &PyMCM::generate_data_file, py::arg("N_samples"), py::arg("data"), py::arg("filename"))
        .def("generate_data_object", &PyMCM::generate_data_object, py::arg("N_samples"), py::arg("data"))
        .def_property("array", &PyMCM::get_partition_array, &PyMCM::set_partition_array)
        .def_property("array_gray_code", &PyMCM::get_partition_as_gray_code, &PyMCM::set_partition_gray_code)
        .def_property_readonly("n", &PyMCM::get_n)
        .def_property_readonly("n_icc", &PyMCM::get_n_comp)
        .def_property_readonly("rank", &PyMCM::get_rank)
        .def_property_readonly("is_optimized", &PyMCM::is_optimized);
}