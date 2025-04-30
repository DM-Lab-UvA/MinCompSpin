#include "utilities/spin_ops.h"

int count_set_bits(__uint128_t value){
    int count = 0;
    while (value){
        value &= (value - 1);
        ++count;
    }
    return count;
}

int spin_value(const std::vector<__uint128_t>& state, const std::vector<__uint128_t>& op, int q){
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

double calc_entropy_of_spin_op(const Data& data, const std::vector<__uint128_t>& op){
    // Variable for the probability distribution
    std::vector<double> prob_distr(data.q, 0);

    int s;
    for (const std::pair<std::vector<__uint128_t>, unsigned int>& datapoint : data.dataset){
        s = spin_value(datapoint.first, op, data.q);
        prob_distr[s] += datapoint.second;
    }
    // Calculate the entropy
    double N = data.N;
    double p;
    double entropy = 0;
    for (int val : prob_distr){
        if(val){
            p = val / N;
            entropy -= p * log(p);
        }
    }
    return entropy;
}

bool comp_entropy(const std::pair<std::vector<uint8_t>, double>& op1, const std::pair<std::vector<uint8_t>, double>& op2){
    return op1.second < op2.second;
}

bool valid_operator(const std::vector<uint8_t>& op, int q){
    // At least one power of the operator must be coprime with q (otherwise it reduces the state space)
    // The first power coprime with q should be smaller than (q/2 + 1) because this removes one operator from each conjugate pair
    // Start from the last variable because the generate_colors function already eliminates some conjugate operators
    for (int k = op.size() - 1; k > -1; --k){
        int op_k = op[k];
        if (op_k){
            if (gcd(op_k, q) == 1){
                if(op_k <= (q - op_k)){
                    return true;
                }
                else {
                    return false;
                }
            }
        }
    }
    return false;
}

void generate_colors_of_spin_ops(std::vector<std::vector<uint8_t>>& colors, int order, int q){
    // Start with all variables raised to the first power
    std::vector<uint8_t> op(order, 1);

    // Stop algorithm if the last variable has the power (q/2 + 1) because the remaining operators form a pair with a previously generated operator
    int stop_index = (q / 2) + 1;

    while (true){
        if (valid_operator(op, q)){
            colors.push_back(op);
        }
        // Increase the power of the first variable
        ++op[0];

        // Max power can only be q-1
        for (int i = 0; i < order-1; ++i){
            if (op[i] == q){
                ++op[i + 1];
                op[i] = 1;
            }
        }

        // Stopping criteria
        if (op[order-1] == stop_index){break;}
    }
}

void entropy_of_spin_ops_of_order_k(const Data& data, std::vector<std::pair<std::vector<uint8_t>, double>>& entropy_of_ops, int order){
    // First binary operator: k first bits set to 1
    __uint128_t ONE = 1;
    __uint128_t op_bin = (ONE << order) - 1;

    // Variables to generate all binary operators
    __uint128_t c;
    __uint128_t c_stop = ONE << (data.n - order);

    // Generate all possible colors of spin operators for k variables
    std::vector<std::vector<uint8_t>> colors;
    generate_colors_of_spin_ops(colors, order, data.q); 

    // Create variables to keep track of the operators and entropy
    __uint128_t op_tmp;
    std::vector<__uint128_t> op128(data.n_ints, 0);
    std::vector<uint8_t> op(data.n, 0);
    std::pair<std::vector<uint8_t>, double> entropy_of_op;

    while (true){
        // Loop over all possible colors for this set of k variables
        for (std::vector<uint8_t> color : colors){
            op_tmp = op_bin;
            int j = 0;
            int i = 0;
            // Write spin operator as vector of n number between 0 and q-1
            while (op_tmp){
                if (op_tmp & 1){
                    op[j] = color[i];
                    ++i;
                }
                ++j;
                op_tmp >>= 1;
            }
            // Put the operator in different representation to calculate the entropy
            convert_8bit_vec_to_128bit_vec_unsafe(op128, op, data.n, data.q);
            // Calculate the entropy
            entropy_of_op.first = op;   
            entropy_of_op.second = calc_entropy_of_spin_op(data, op128);
            entropy_of_ops.push_back(entropy_of_op);
        }
        // Reset operator
        std::fill(op.begin(), op.end(), 0);

        // Generate next set of variables

        // Find position of the lowest bit
        c = (((op_bin) - 1) ^ op_bin) & op_bin;
        // Check if this is the final operator
        if (c == c_stop){break;}
        // Increment the operator by 1 from the lowest bit
        op_bin += c;

        // Fill the operator with 1's starting from bit 0
        while (bit_count(op_bin) < order){
            ++op_bin;
            // Bit 0 is now the lowest bit
            c = 1;
        }
    }
}

void sort_spin_ops(const Data& data, std::vector<std::vector<uint8_t>>& spin_ops, int max_order){
    std::vector<std::pair<std::vector<uint8_t>, double>> entropy_of_ops;
    // Loop over all orders of spin operators up to max_order
    // Calculate and store the entropy
    for (int k = 1; k <= max_order; ++k){
        entropy_of_spin_ops_of_order_k(data, entropy_of_ops, k);
    }

    // Sort the operators from low to high entropy
    __uint128_t n_ops = entropy_of_ops.size();
    spin_ops.assign(data.n, std::vector<uint8_t>(n_ops));
    sort(entropy_of_ops.begin(), entropy_of_ops.end(), comp_entropy);

    // Create the spin ops matrix
    __uint128_t j = 0;
    for (std::pair<std::vector<uint8_t>, double>& pair: entropy_of_ops){
        for (int i = 0; i < data.n; ++i){
            spin_ops[i][j] = pair.first[i];
        }
        ++j;
    }
}
    