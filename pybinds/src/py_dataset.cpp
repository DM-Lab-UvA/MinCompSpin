#include "py_dataset.h"

double PyData::calc_log_ev_array(py::array_t<int8_t> partition){
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
    return this->data.calc_log_ev(conv_partition);
}

double PyData::calc_log_ev_model(PyModel& model){
    if (model.get_n() != this->get_n()){
        throw std::invalid_argument("The system size of the model and the dataset don't match.");
    }
    return this->data.calc_log_ev(model.model.get_partition());
}

py::array PyData::calc_log_ev_icc(PyModel& model){
    if (model.get_n() != this->get_n()){
        throw std::invalid_argument("The system size of the model and the dataset don't match.");
    }
    std::vector<__uint128_t> partition = model.model.get_partition();
    std::vector<double> log_ev_per_icc;

    for (int i = 0; i < partition.size(); i++){
        if (partition[i]){
            log_ev_per_icc.push_back(this->data.calc_log_ev_icc(partition[i]));
        }
    }
    return py::array(log_ev_per_icc.size(), log_ev_per_icc.data());    

}

py::array PyData::calc_log_ev_icc_array(py::array_t<int8_t> partition){
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

    std::vector<double> log_ev_per_icc;
    for (int i = 0; i < conv_partition.size(); i++){
        if (conv_partition[i]){
            log_ev_per_icc.push_back(this->data.calc_log_ev_icc(conv_partition[i]));
        }
    }
    return py::array(log_ev_per_icc.size(), log_ev_per_icc.data());
}

double PyData::entropy(int base){
    return this->data.entropy(base);
}


void bind_data_class(py::module &m) {
    py::class_<PyData>(m, "Data")
        .def(py::init<const std::string&, int, int>())
        .def("log_evidence_icc", &PyData::calc_log_ev_icc)
        .def("log_evidence_icc", &PyData::calc_log_ev_icc_array)
        .def("log_evidence", &PyData::calc_log_ev_model)
        .def("log_evidence", &PyData::calc_log_ev_array)
        .def("entropy", &PyData::entropy, py::arg("base") = -1)
        .def_property_readonly("n", &PyData::get_n)
        .def_property_readonly("q", &PyData::get_q)
        .def_property_readonly("N", &PyData::get_N)
        .def_property_readonly("N_unique", &PyData::get_N_unique);
}
