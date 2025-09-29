#include "py_dataset.h"

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
        .def_property_readonly("N_unique", &PyData::get_N_unique)
        .def_property("N_synthetic", &PyData::get_N_synthetic, &PyData::set_N_synthetic);
}
