#include "py_model.h"

PyModel::PyModel(int n, py::array_t<int8_t> partition) : model(n) {
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
    this->model.set_starting_partition(conv_partition);
}

py::array_t<int8_t> PyModel::get_partition_array() {
    int n = this->model.n;
    int n_comp = this->model.n_comp;
    return convert_partition_to_py(this->model.get_partition(), n, n_comp);
}

void PyModel::set_partition_array(py::array_t<int8_t> partition) {
    std::vector<__uint128_t> conv_partition = convert_partition_from_py_2d_array(partition);
    this->model.set_starting_partition(conv_partition);
}

py::array_t<int8_t> PyModel::get_partition_as_gray_code() {
    int n_comp = this->model.n_comp;
    return convert_partition_to_py_gray_code(this->model.get_partition(), n_comp);
}

void PyModel::set_partition_gray_code(py::array_t<int8_t> partition) {
    std::vector<__uint128_t> conv_partition = convert_partition_from_py_gray_code(partition, this->model.n);
    this->model.set_starting_partition(conv_partition);
}

void PyModel::move_var_in(int var_index, int comp_index){
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
        if (var & this->model.get_partition()[i]){
            throw std::invalid_argument("This variable is already present in the partition.");
        }
    }
    // Add the variable
    if (!this->model.partition[comp_index]){
        // Moved to a new component
        this->model.n_comp++;
    }
    this->model.partition[comp_index] += var;
    this->model.rank++;

    // Reset if a search has been ran before
    if (this->model.optimized){
        this->model.optimized = false;
        this->model.log_ev_per_icc.clear();
    } 
}

void PyModel::move_var_out(int var_index){
    // Check if the variable index is valid
    if (var_index >= this->get_n()){
        throw std::invalid_argument("The variable index should be between 0 and n-1.");
    }
    __uint128_t var = 1 << var_index;
    int n_comp = this->get_n_comp();
    int i = 0;
    // Find the component that contains this variable
    while(! (var & this->model.get_partition()[i])){
        if (i == n_comp-1){
            throw std::invalid_argument("This variable is not present in the partition.");
        }
        i++;
    }
    // Remove the variable
    this->model.partition[i] -= var;
    this->model.rank--;

    // Reduce the number of components
    if (! this->model.partition[i]){
        this->model.n_comp--;
        place_empty_component_last(this->model.partition);
    }
    // Reset if a search has been ran before
    if (this->model.optimized){
        this->model.optimized = false;
        this->model.log_ev_per_icc.clear();
    } 
}

void PyModel::move_var(int var_index, int comp_index){
    this->move_var_out(var_index);
    this->move_var_in(var_index, comp_index);

    // Reset if a search has been ran before
    if (this->model.optimized){
        this->model.optimized = false;
        this->model.log_ev_per_icc.clear();
    }
}

double PyModel::get_best_log_ev() {
    return this->model.get_best_log_ev();
}

py::array PyModel::get_best_log_ev_per_icc() {
    std::vector<double> ev_per_icc = this->model.get_best_log_ev_per_icc();
    return py::array(this->model.n_comp, ev_per_icc.data());
}

void bind_model_class(py::module &m) {
    py::class_<PyModel>(m, "Model")
        .def(py::init<int>())
        .def(py::init<int, py::array_t<int8_t>>())
        .def(py::init<int, const std::string&>())
        .def_property("array", &PyModel::get_partition_array, &PyModel::set_partition_array)
        .def_property("array_gray_code", &PyModel::get_partition_as_gray_code, &PyModel::set_partition_gray_code)
        .def("move_variable_in", &PyModel::move_var_in)
        .def("move_variable_out", &PyModel::move_var_out)
        .def("move_variable", &PyModel::move_var)
        .def("get_best_log_evidence", &PyModel::get_best_log_ev)
        .def("get_best_log_evidence_icc", &PyModel::get_best_log_ev_per_icc)
        .def_property_readonly("n", &PyModel::get_n)
        .def_property_readonly("n_icc", &PyModel::get_n_comp)
        .def_property_readonly("rank", &PyModel::get_rank)
        .def_property_readonly("is_optimized", &PyModel::is_optimized);
}