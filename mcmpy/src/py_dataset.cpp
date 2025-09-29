#include "py_dataset.h"

PyData::PyData(py::array_t<int> input_array, int n_var, int n_states)
    : data(std::vector<std::pair<std::vector<__uint128_t>, unsigned int>>(),
           n_var, n_states, 0)
{
    // Check if the given number of variables is valid
    if (n_var > 128) {
        throw std::invalid_argument("The maximum system size is 128 variables.");
    }
    if (n_var < 1) {
        throw std::invalid_argument("The system size should be at least 1.");
    }
    
    // Assign variables
    this->data.n = n_var;
    this->data.q = n_states;
    
    // Calculate the number of integers necessary to represent the data
    this->data.n_ints = ceil(log2(this->data.q));
    
    // Process the dataset from NumPy array
    this->data.N = processing_numpy(
                input_array,
                n_var,
                this->data.n_ints,
                n_states,
                this->data.dataset
            );
    this->data.N_unique = this->data.dataset.size();
    
    // Precompute the powers of q to speed up the calculation of the evidence (q^r)
    this->data.pow_q.assign(data.n+1, 0);
    __uint128_t element = 1;
    for(int i = 0; i < data.n+1; i++){
        this->data.pow_q[i] = element;
        element *= this->data.q;
    }
}

int PyData::processing_numpy(py::array_t<int> input_array, int n_var, int n_ints, int n_states, 
                            std::vector<std::pair<std::vector<__uint128_t>, unsigned int>>& data) {
    
    py::buffer_info buf = input_array.request();
    
    // Check array dimensionality
    if (buf.ndim != 2) {
        throw std::invalid_argument("Input array must be 2-dimensional");
    }
    
    int num_rows = buf.shape[0];
    int num_cols = buf.shape[1];
    
    // Check array has enough rows
    if (num_rows == 0) {
        throw std::invalid_argument("Array must have at least one row");
    }
    
    // Check array has enough columns for n_var
    if (num_cols < n_var) {
        throw std::invalid_argument("Array must have at least " + std::to_string(n_var) + 
                                  " columns (n_var=" + std::to_string(n_var) + 
                                  "), but got " + std::to_string(num_cols) + " columns");
    }
    
    // Pre-validate all values are in valid range [0, n_states-1]
    int* ptr = static_cast<int*>(buf.ptr);
    for (int row = 0; row < num_rows; row++) {
        for (int col = 0; col < n_var; col++) {
            int value = ptr[row * num_cols + col];
            if (value < 0 || value >= n_states) {
                throw std::invalid_argument("Array value " + std::to_string(value) + 
                                          " at position [" + std::to_string(row) + 
                                          ", " + std::to_string(col) + 
                                          "] is out of valid range [0, " + 
                                          std::to_string(n_states - 1) + "] for n_states=" + 
                                          std::to_string(n_states));
            }
        }
    }
    
    std::unordered_map<std::vector<__uint128_t>, unsigned int, HashVector128> dataset_map;
    int N = 0;
    std::vector<__uint128_t> observation(n_ints);
    
    // Process each row (values are already validated)
    for (int row = 0; row < num_rows; row++) {
        // Reset observation vector
        std::fill(observation.begin(), observation.end(), 0);
        __uint128_t element = 1;
        
        for (int i = 0; i < n_var; i++) {
            int value = ptr[row * num_cols + i];
            
            // Convert value to binary representation
            int bit = 0;
            while (value) {
                if (value & 1) {
                    observation[bit] += element;
                }
                ++bit;
                value >>= 1;
            }
            element <<= 1;
        }
        
        dataset_map[observation]++;
        N++;
    }
    
    // Convert map to vector
    for (auto &my_pair : dataset_map) {
        data.push_back(my_pair);
    }
    
    return N;
}

std::vector<__uint128_t> convert_spin_op_from_py(const py::array_t<uint8_t>& spin_op, int q, int n_ints){
    py::buffer_info buff = spin_op.request();

    // Check if there is only one dimension
    int ndim = buff.ndim;
    if (ndim != 1){
        throw std::invalid_argument("The spin operator should be given as a 1D numpy array.");
    }
    // Check if the system size is valid
    int n = buff.shape[0];
    if (n > 128){
        throw std::invalid_argument("The maximum system size is 128.");
    }

    std::vector<uint8_t> conv_spin_op(n, 0);
    uint8_t *ptr = static_cast<uint8_t *>(buff.ptr);

    for (int i = 0; i < n; ++i){
        conv_spin_op[i] = ptr[i];
    }

    std::vector<__uint128_t> op(n_ints, 0);
    convert_8bit_vec_to_128bit_vec(op, conv_spin_op, n, q);
    return op;
}

double PyData::calc_property_array(py::array_t<int8_t> partition, std::string property){
    std::vector<__uint128_t> conv_partition;

    // Check the dimensions of the array
    py::buffer_info buff = partition.request();
    int ndim = buff.ndim;

    if (ndim == 1){
        conv_partition = convert_partition_from_py_gray_code(partition, this->get_n());
    }
    else if (ndim == 2){
        conv_partition = convert_partition_from_py_2d_array(partition);
    }
    else{
        throw std::invalid_argument("The partition should be a 1D or 2D array.");
    }

    if (property == "evidence"){return this->data.calc_log_ev(conv_partition);}
    else if (property == "likelihood"){return this->data.calc_log_likelihood(conv_partition);}
    else if (property == "geom_compl"){return this->data.calc_geom_complexity(conv_partition);}
    else if (property == "param_compl"){return this->data.calc_param_complexity(conv_partition);}
    else{return this->data.calc_mdl(conv_partition);}
}

double PyData::calc_property_mcm(PyMCM& mcm, std::string property){
    if (mcm.get_n() != this->get_n()){
        throw std::invalid_argument("The system size of the mcm and the dataset don't match.");
    }
    if (property == "evidence"){return this->data.calc_log_ev(mcm.mcm.get_partition());}
    else if (property == "likelihood"){return this->data.calc_log_likelihood(mcm.mcm.get_partition());}
    else if (property == "geom_compl"){return this->data.calc_geom_complexity(mcm.mcm.get_partition());}
    else if (property == "param_compl"){return this->data.calc_param_complexity(mcm.mcm.get_partition());}
    else{return this->data.calc_mdl(mcm.mcm.get_partition());}
}

py::array PyData::calc_property_icc_array(py::array_t<int8_t> partition, std::string property){
    std::vector<__uint128_t> conv_partition;

    // Check the dimensions of the array
    py::buffer_info buff = partition.request();
    int ndim = buff.ndim;

    if (ndim == 1){
        conv_partition = convert_partition_from_py_gray_code(partition, this->get_n());
    }
    else if (ndim == 2){
        conv_partition = convert_partition_from_py_2d_array(partition);
    }
    else{
        throw std::invalid_argument("The partition should be a 1D or 2D array.");
    }

    std::vector<double> property_per_icc;

    if (property == "evidence"){
        for (int i = 0; i < conv_partition.size(); i++){
            if (conv_partition[i]){
                property_per_icc.push_back(this->data.calc_log_ev_icc(conv_partition[i]));
            }
        }
    }
    else if (property == "likelihood"){
        for (int i = 0; i < conv_partition.size(); i++){
            if (conv_partition[i]){
                property_per_icc.push_back(this->data.calc_log_likelihood_icc(conv_partition[i]));
            }
        }
    }
    else if (property == "geom_compl"){
        for (int i = 0; i < conv_partition.size(); i++){
            if (conv_partition[i]){
                property_per_icc.push_back(this->data.calc_geom_complexity_icc(conv_partition[i]));
            }
        }
    }
    else if (property == "param_compl"){
        for (int i = 0; i < conv_partition.size(); i++){
            if (conv_partition[i]){
                property_per_icc.push_back(this->data.calc_param_complexity_icc(conv_partition[i]));
            }
        }
    }
    return py::array(property_per_icc.size(), property_per_icc.data());
}

py::array PyData::calc_property_icc(PyMCM& mcm, std::string property){
    if (mcm.get_n() != this->get_n()){
        throw std::invalid_argument("The system size of the mcm and the dataset don't match.");
    }
    std::vector<__uint128_t> partition = mcm.mcm.get_partition();
    std::vector<double> property_per_icc;

    if (property == "evidence"){
        for (int i = 0; i < partition.size(); i++){
            if (partition[i]){
                property_per_icc.push_back(this->data.calc_log_ev_icc(partition[i]));
            }
        }
    }
    else if (property == "likelihood"){
        for (int i = 0; i < partition.size(); i++){
            if (partition[i]){
                property_per_icc.push_back(this->data.calc_log_likelihood_icc(partition[i]));
            }
        }
    }
    else if (property == "geom_compl"){
        for (int i = 0; i < partition.size(); i++){
            if (partition[i]){
                property_per_icc.push_back(this->data.calc_geom_complexity_icc(partition[i]));
            }
        }
    }
    else if (property == "param_compl"){
        for (int i = 0; i < partition.size(); i++){
            if (partition[i]){
                property_per_icc.push_back(this->data.calc_param_complexity_icc(partition[i]));
            }
        }
    }
    return py::array(property_per_icc.size(), property_per_icc.data());    
}

double PyData::entropy(int base){
    return this->data.entropy(base);
}

double PyData::entropy_of_spin_op(const py::array_t<int8_t>& op){
    std::vector<__uint128_t> spin_op = convert_spin_op_from_py(op, this->data.q, this->data.n_ints);
    return calc_entropy_of_spin_op(this->data, spin_op);
}


void bind_data_class(py::module &m) {
    py::class_<PyData>(m, "Data")
        .def(py::init<const std::string&, int, int>())
        .def(py::init<py::array_t<int>, int, int>())
        .def("log_evidence_icc", &PyData::calc_log_ev_icc, py::arg("mcm"))
        .def("log_evidence_icc", &PyData::calc_log_ev_icc_array, py::arg("partition"))
        .def("log_evidence", &PyData::calc_log_ev_mcm)
        .def("log_evidence", &PyData::calc_log_ev_array)

        .def("log_likelihood_icc", &PyData::calc_log_likelihood_icc)
        .def("log_likelihood_icc", &PyData::calc_log_likelihood_icc_array)
        .def("log_likelihood", &PyData::calc_log_likelihood_mcm)
        .def("log_likelihood", &PyData::calc_log_likelihood_array)

        .def("complexity_geometric_icc", &PyData::calc_geom_complexity_icc)
        .def("complexity_geometric_icc", &PyData::calc_geom_complexity_icc_array)
        .def("complexity_geometric", &PyData::calc_geom_complexity_mcm)
        .def("complexity_geometric", &PyData::calc_geom_complexity_array)

        .def("complexity_parametric_icc", &PyData::calc_param_complexity_icc)
        .def("complexity_parametric_icc", &PyData::calc_param_complexity_icc_array)
        .def("complexity_parametric", &PyData::calc_param_complexity_mcm)
        .def("complexity_parametric", &PyData::calc_param_complexity_array)

        .def("minimum_description_length", &PyData::calc_mdl_array)
        .def("minimum_description_length", &PyData::calc_mdl_mcm)

        .def("entropy", &PyData::entropy, py::arg("base") = -1)
        .def("entropy_of_spin_operator", &PyData::entropy_of_spin_op, py::arg("spin_op"))
        .def_property_readonly("n", &PyData::get_n)
        .def_property_readonly("q", &PyData::get_q)
        .def_property_readonly("N", &PyData::get_N)
        .def_property_readonly("N_unique", &PyData::get_N_unique);
}
