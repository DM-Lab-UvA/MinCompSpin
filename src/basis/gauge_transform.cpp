#include "basis/basis.h"

int count_set_bits(__uint128_t value){
    int count = 0;
    while (value){
        value &= (value - 1);
        ++count;
    }
    return count;
}

int spin_value(std::vector<__uint128_t>& state, std::vector<__uint128_t>& op, int q){
    // s = sum(alpha_j * mu_j)
    int s = 0;
    int element_j = 1;
    for (__uint128_t j : op){
        int element_i = 1;
        for (__uint128_t i : state){
            s += element_i * element_j * count_set_bits(i & j);
            element_i <<= 1;
        }
        element_j <<= 1;
    }
    return s % q;
}

void Basis::gt_data_in_place(Data& data) {
    // Check if the number of variables match
    if (this->n != data.n){
        throw std::invalid_argument("Number of variables in the data doesn't match the number of variables in the basis.");
    }
    // Check the value of q
    if (this->q != data.q){
        throw std::invalid_argument("Number of values each variable can take in the data doesn't match the number of values each variable can take in the basis.");
    }
    // Loop over the different states in the data
    int bit;
    int spin_val;
    __uint128_t element;
    std::vector<__uint128_t> gt_state(this->n_ints, 0);

    for (std::pair<std::vector<__uint128_t>, unsigned int>& datapoint : data.dataset){
        // GT of the state
        std::fill(gt_state.begin(), gt_state.end(), 0);
        element = 1;
        for (std::vector<__uint128_t>& op : this->basis_ops){
            // Loop over the integers of the operator and state to determine the spin value in the transformed state
            spin_val = spin_value(datapoint.first, op, this->q);
            // Add to the converted state
            bit = 0;
            while (spin_val){
                // Check if last bit in the binary representation is nonzero
                if (spin_val & 1){
                    gt_state[bit] += element;
                }
                // Bitshift to the right
                ++bit;
                spin_val >>= 1;
            }
            // Bitshift to the left to get the decimal value of the next variable
            element <<= 1;
        }
        // Update the dataset
        datapoint.first = gt_state;
    }
}

Data Basis::gt_data(Data& data) {
    // Copy the given dataset
    Data gt_data = data;
    // Transform the copy in place
    this->gt_data_in_place(gt_data);
    return gt_data;
}
