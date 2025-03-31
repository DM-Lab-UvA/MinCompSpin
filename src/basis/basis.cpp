#include "basis/basis.h"

Basis::Basis(int n_var, int n_states) : basis_ops_matrix(n_var, std::vector<uint8_t>(n_var)) {
    // Check if the given number of variables is valid
    if (n_var < 1){
        throw std::invalid_argument("The number of variables should be a non-zero positive number.");
    }
    if (n_var > 128){
        throw std::invalid_argument("The maximum system size is 128 variables.");
    }

    // Check if the number of states is valid
    if (n_states < 2){
        throw std::invalid_argument("The number of states should be at least 2.");
    }
    this->n = n_var;
    this->q = n_states;
    this->n_ints = ceil(log2(n_states));
    // Set the default base: s1, s2, ..., sn
    std::vector<__uint128_t> spin_op(this->n_ints, 0);
    __uint128_t element = 1;
    for (int i = 0; i < n_var; i++){
        // log2(q) 128 bit integer representation
        spin_op[0] = element;
        this->basis_ops.push_back(spin_op);
        element <<= 1;
        // n 8bit integer representation
        this->basis_ops_matrix[i][i] = 1;
    }
}

Basis::Basis(int n_var, int n_states, const std::vector<std::vector<uint8_t>>& spin_ops) : basis_ops_matrix(n_var, std::vector<uint8_t>(n_var)){
    // Check if the given number of variables is valid
    if (n_var < 1){
        throw std::invalid_argument("The number of variables should be a non-zero positive number.");
    }
    if (n_var > 128){
        throw std::invalid_argument("The maximum system size is 128 variables.");
    }

    // Check if the number of states is valid
    if (n_states < 2){
        throw std::invalid_argument("The number of states should be at least 2.");
    }
    this->n = n_var;
    this->q = n_states;
    this->n_ints = ceil(log2(n_states));

    this->set_basis(spin_ops);
}

Basis::Basis(int n_var, int n_states, std::string& file) : basis_ops_matrix(n_var, std::vector<uint8_t>(n_var)) {
    // Check if the given number of variables is valid
    if (n_var < 1){
        throw std::invalid_argument("The number of variables should be a non-zero positive number.");
    }
    if (n_var > 128){
        throw std::invalid_argument("The maximum system size is 128 variables.");
    }

    // Check if the number of states is valid
    if (n_states < 2){
        throw std::invalid_argument("The number of states should be at least 2.");
    }
    this->n = n_var;
    this->q = n_states;
    this->n_ints = ceil(log2(n_states));
    
    this->set_basis_from_file(file);
}

void Basis::set_basis(const std::vector<std::vector<uint8_t>>& spin_ops){
    // Check the dimension of the given base
    if (spin_ops.size() != this->n){
        throw std::invalid_argument("The given basis doesn't have n spin operators.");
    }
    // Copy the given basis
    this->basis_ops.clear();
    std::vector<__uint128_t> spin_op(this->n_ints, 0);
    std::vector<uint8_t> spin_op_array;
    for (int i = 0; i < spin_ops.size(); i++){
        spin_op_array = spin_ops[i];
        // Check if there are n entries
        if (spin_op_array.size() != this->n){
            throw std::invalid_argument("Invalid spin operator in the given basis because it does not contain n variables.");
        }
        convert_8bit_vec_to_128bit_vec(spin_op, spin_op_array, this->n, this->q);
        this->basis_ops.push_back(spin_op);
        for (int j = 0; j < this->n; ++j){
            this->basis_ops_matrix[j][i] = spin_op_array[j];
        }
    } 
    // Check if the basis in linearly independent
    if (! is_linearly_independent(this->basis_ops_matrix, this->n, this->q)){
        throw std::invalid_argument("Given basis is not linearly independent.");
    }
}

void Basis::set_basis_from_file(std::string& file){
    // Open file
    std::ifstream myfile(file);

    // Check if file exists
    if (myfile.fail()){
        throw std::invalid_argument("Not able to open the file.");
    }

    // Clear previous basis
    this->basis_ops.clear();

    // Counter for the number of lines/operators
    int n_ops = 0;

    std::string line;
    std::vector<__uint128_t> spin_op(this->n_ints);
    while (getline(myfile, line)){
        // Check if there are at least n characters in a line
        if (line.size() != this->n){
            throw std::invalid_argument("File contains invalid spin operator because it does not contain n variables.");
        }
        // Convert from string of n values between 0 and q-1 to a vector of log2(q) 128bit integers
        convert_string_to_vector(spin_op, line, this->n, this->q);
        this->basis_ops.push_back(spin_op);

        // Matrix representation
        for(int i = 0; i < this->n; i++){
            this->basis_ops_matrix[i][n_ops] = line[i] - '0';
        }
        n_ops++;
    }
    // Check if there there are n spin operators given
    if (n_ops != this->n){
        throw std::invalid_argument("The given basis doesn't have n spin operators.");
    }
    // Check if the basis in linearly independent
    if (! is_linearly_independent(this->basis_ops_matrix, this->n, this->q)){
        throw std::invalid_argument("Given basis is not linearly independent.");
    }
}