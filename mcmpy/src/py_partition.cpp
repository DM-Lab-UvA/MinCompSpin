#include "py_partition.h"

py::array_t<int8_t> convert_partition_to_py(std::vector<__uint128_t>& partition, int n, int n_comp){
    // Initialize 2d python array
    py::array_t<uint8_t> conv_partition = py::array_t<uint8_t>({n_comp, n});
    
    py::buffer_info buff = conv_partition.request();
    uint8_t *ptr = static_cast<uint8_t *>(buff.ptr);

    __uint128_t comp;
    for (int i = 0; i < n_comp; i++){
        comp = partition[i];
        for (int j = 0; j < n; j++) {
            if (comp & 1){
                ptr[i*n + j] = 1;
            }
            else{
                ptr[i*n + j] = 0;
            }
            comp >>= 1;
        }
    }
    return conv_partition;
}

py::array_t<int8_t> convert_partition_to_py_gray_code(std::vector<__uint128_t>& partition, int n_comp){
    // Initialize a 1d python array
    int n = partition.size();
    py::array_t<int8_t> conv_partition = py::array_t<int8_t>(n);

    py::buffer_info buff = conv_partition.request();
    int8_t *ptr = static_cast<int8_t *>(buff.ptr);
    // Set every variable as not present in the partition (component index -1)
    for (int i = 0; i < n; i++){
        ptr[i] = -1;
    }

    int j;
    __uint128_t comp;
    for (int i = 0; i < n_comp; i++){
        comp = partition[i];
        j = 0;
        while (comp){
            if(comp & 1){
                ptr[j] = i;
            }
            comp >>= 1;
            j++;
        }        
    }
    return conv_partition; 
}

std::vector<__uint128_t> convert_partition_from_py_2d_array(py::array_t<int8_t>& py_partition){
    py::buffer_info buff = py_partition.request();

    // Check the dimensions of the array
    int ndim = buff.ndim;
    if (ndim != 2){
        throw std::invalid_argument("The partition should be given as a 2 dimensional array.");
    }

    // Check if the system size is valid
    int n = buff.shape[1];
    int n_comp = buff.shape[0];
    if (n > 128){
        throw std::invalid_argument("The maximum system size is 128.");
    }

    std::vector<__uint128_t> partition(n, 0);

    int8_t *ptr = static_cast<int8_t *>(buff.ptr);
    __uint128_t element;

    for (int i = 0; i < n_comp; i++){
        element = 1;
        for (int j = 0; j < n; j++){
            if(ptr[i*n + j]){
                if(ptr[i*n + j] != 1){
                    throw std::invalid_argument("Entries of the 2D array should be either 0 or 1.");
                }
                partition[i] += element;
            }
            element <<= 1;
        }
    }
    return partition;
}

std::vector<__uint128_t> convert_partition_from_py_gray_code(py::array_t<int8_t>& py_partition, int n){
    py::buffer_info buff = py_partition.request();

    // Check the dimensions of the array
    int ndim = buff.ndim;
    if (ndim != 1){
        throw std::invalid_argument("The partition should be given as a 1 dimensional array.");
    }

    // Check if the system size is valid
    int n_var = buff.shape[0];
    if (n_var > n){
        throw std::invalid_argument("Invalid partition because there are more than n variables.");
    }

    std::vector<__uint128_t> partition(n, 0);

    int8_t *ptr = static_cast<int8_t *>(buff.ptr);
    __uint128_t element = 1;

    for (int i = 0; i < n_var; i++){
        int comp_index = ptr[i];

        if (comp_index >= n){
            throw std::invalid_argument("Invalid partition because one of the given component indices is equal or langer than n.");
        }
        else if (comp_index >=0){
            partition[comp_index] += element;
        }
        element <<= 1;
    }
    return partition;
}

__uint128_t convert_component_from_py(py::array_t<uint8_t>& component){
    py::buffer_info buff = component.request();

    int ndim = buff.ndim;

    if (ndim != 1){
        throw std::invalid_argument("The component should be a 1 dimensional array.");
    }
    int n = buff.shape[0];

    if (n > 128){
        throw std::invalid_argument("The component can contain at most 128 variables.");
    }

    __uint128_t element = 1;
    __uint128_t conv_component = 0;
    uint8_t *ptr = static_cast<uint8_t *>(buff.ptr);

    for (int i = 0; i < n; i++){
        if (ptr[i]){
            conv_component += element;
        }
        element <<= 1;
    }
    return conv_component;
}