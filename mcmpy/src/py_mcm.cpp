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
        .def(py::init<int>(), py::arg("n"), 
        R"pbdoc(
        Construct a new MCM object with a given number of variables.
        
        The default partition is the independent model, where each variable is placed in its own component.
        
        Parameters
        ----------
        n : int
            Number of variables in the system.
        )pbdoc")
        .def(py::init<int, py::array_t<int8_t>>(), py::arg("n"), py::arg("partition"), 
        R"pbdoc(
        Construct a new MCM object with a given number of variables and a specific partition.
        
        The partition must be given as a 2D binary array or a 1D gray code array.
        See the corresponding attributes below for an example.
        
        - In the 2D representation each row represents an ICC and each column a variable.
          If a variable is present in a component, the corresponding element is one and zero otherwise.
        
        - In the 1D gray code representation, the elements correspond to the component index in which each variable is present. 
          To indicate that a variable is not present in the model, the component index -1 is used.
        
        Parameters
        ----------
        n : int
            Number of variables in the system.
        partition : numpy.ndarray
            Partition as an integer array of shape (n,) representing component assignments.
        )pbdoc")
        .def(py::init<int, const std::string&>(), py::arg("n"), py::arg("partition"), 
        R"pbdoc(
        Construct a new MCM object with a given number of variables and a partition type.
        
        Valid partition types:
            - "independent" : All variables are placed in separate components.
            - "complete" : All variables are placed in a single component.
            - "random" : Each variable is assigned to a random component.
        
        Parameters
        ----------
        n : int
            Number of variables in the system.
        partition : str
            String indicating the type of partition.
        )pbdoc")
        .def_property("array", &PyMCM::get_partition_array, &PyMCM::set_partition_array, "The partition of the MCM represented as a 2D binary array.")
        .def_property("array_gray_code", &PyMCM::get_partition_as_gray_code, &PyMCM::set_partition_gray_code, "The partition of the MCM represented as a 1D gray code array.")
        .def("move_variable_in", &PyMCM::move_var_in, py::arg("var_index"), py::arg("comp_index"),
        R"pbdoc(
        Add a variable to a specified component in the partition.

        The variable index must be between 0 and n - 1 and refer to a variable that is not currently assigned to any component.
        The component index must also be between 0 and n - 1.
        If the specified component index exceeds the index of the first empty component, the variable will instead be added to the first available empty component.
        **Note** that this function will reset the optimized status of the MCM if it was the result of an MCMSearch before.
        The functions 'print_info', 'get_best_log_evidence' and 'get_best_log_evidence_icc' will no longer be available.

        Parameters
        ----------
        var_index : int
            Index of the variable to add.
        comp_index : int
            Index of the component to where the variable will move.
        )pbdoc"
        )
        .def("move_variable_out", &PyMCM::move_var_out, py::arg("var_index"),
        R"pbdoc(
        Remove a specific variable from the partition.

        The variable index must be between 0 and n - 1 and refer to a variable that is currently assigned to a component in the partition.
        After removal, the variable will no longer be associated with any component.
        **Note** that this function will reset the optimized status of the MCM if it was the result of an MCMSearch before.
        The functions 'print_info', 'get_best_log_evidence' and 'get_best_log_evidence_icc' will no longer be available.

        Parameters
        ----------
        var_index : int
            Index of the variable to remove.
        )pbdoc")
        .def("move_variable", &PyMCM::move_var, py::arg("var_index"), py::arg("comp_index"),
        R"pbdoc(
        Move a specific variable to another component.

        The variable index should be a value between 0 and n-1 and refer to a variable that is present in the partition.
        The given component index should be a value between 0 and n-1. 
        If the specified component index exceeds the index of the first empty component, the variable will instead be moved to the first available empty component.
        **Note** that this function will reset the optimized status of the MCM if it was the result of an MCMSearch before.
        The functions 'print_info', 'get_best_log_evidence' and 'get_best_log_evidence_icc' will no longer be available.

        Parameters
        ----------
        var_index : int
            Index of the variable to move.
        comp_index : int
            Index of the component to where the variable will move.
        )pbdoc")
        .def("get_best_log_evidence", &PyMCM::get_best_log_ev,
        R"pbdoc(
        Returns the best log-evidence of the MCM.

        Works only after a search for the best mcm has been ran for a given dataset. 

        Returns
        -------
        float
            The log-evidence of the MCM.
        )pbdoc")
        .def("get_best_log_evidence_icc", &PyMCM::get_best_log_ev_per_icc,
        R"pbdoc(
        Returns the best log-evidence of the partition per component.

        Works only after a search for the best mcm has been ran for a given dataset. 

        Returns
        -------
        numpy.ndarray
            Array containing the log-evidence of every component of the MCM.
        )pbdoc")
        .def("print_info", &PyMCM::print_info,
        R"pbdoc(
        Prints out the information about the partition and the log-evidence of the MCM.
        
        Every component of the MCM is printed out as a binary string combined with its log-evidence.
        Works only after a search for the best mcm has been ran for a given dataset. 
        )pbdoc")
        .def("generate_data_file", &PyMCM::generate_data_file, py::arg("N_samples"), py::arg("data"), py::arg("filename"),
        R"pbdoc(
        Generates a file containing N samples from this MCM combined with a given dataset.

        Parameters
        ----------
        N_samples : int
            The number of samples that should be generated.
        data : Data
            The dataset from which the model parameters are inferred.
        filename : string
            Path to the file that will contain the generated data.

        )pbdoc")
        .def("generate_data_object", &PyMCM::generate_data_object, py::arg("N_samples"), py::arg("data"),
        R"pbdoc(
        Generates a Data object containing N samples from this MCM combined with a given dataset.

        Parameters
        ----------
        N_samples : int
            The number of samples that should be generated.
        data : Data
            The dataset from which the model parameters are inferred.
        
        Returns
        -------
        Data
            A Data object containing the generated samples.
        )pbdoc")
        .def_property_readonly("n", &PyMCM::get_n, "The number of variables in the system (read-only).")
        .def_property_readonly("n_icc", &PyMCM::get_n_comp, "The number of independent complete components (ICCs) that the MCM has (read-only).")
        .def_property_readonly("rank", &PyMCM::get_rank, "The number of variables that are present in the MCM (read-only).")
        .def_property_readonly("is_optimized", &PyMCM::is_optimized, "Boolean value indicating whether the MCM is the result from an MCMSearch procedure (read-only).");
}