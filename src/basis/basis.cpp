#include "basis/basis.h"

Basis::Basis(int n_var, int n_states) {
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
    // Set variables
    this->n = n_var;
    this->q = n_states;
    this->n_ints = ceil(log2(n_states));

    // Set the basis
    basis_ops.assign(n_var, std::vector<__uint128_t>(this->n_ints));
    basis_ops_matrix.assign(n_var, std::vector<uint8_t>(n_var));
    this->set_basis_default();
}

Basis::Basis(int n_var, int n_states, const std::vector<std::vector<uint8_t>>& spin_ops) {
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

    // Set the basis
    basis_ops.assign(n_var, std::vector<__uint128_t>(this->n_ints));
    basis_ops_matrix.assign(n_var, std::vector<uint8_t>(n_var));
    this->set_basis(spin_ops);
}

Basis::Basis(int n_var, int n_states, std::string& file) {
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

    // Set the basis
    basis_ops.assign(n_var, std::vector<__uint128_t>(this->n_ints));
    basis_ops_matrix.assign(n_var, std::vector<uint8_t>(n_var));
    this->set_basis_from_file(file);
}

void Basis::set_basis(const std::vector<std::vector<uint8_t>>& spin_ops){
    // Check the dimension of the given base
    if (spin_ops.size() != this->n){
        throw std::invalid_argument("The given basis doesn't have n spin operators.");
    }
    std::vector<__uint128_t> spin_op(this->n_ints, 0);
    std::vector<uint8_t> spin_op_array;
    for (int i = 0; i < spin_ops.size(); i++){
        spin_op_array = spin_ops[i];
        // Check if there are n entries
        if (spin_op_array.size() != this->n){
            throw std::invalid_argument("Invalid spin operator in the given basis because it does not contain n variables.");
        }
        convert_8bit_vec_to_128bit_vec(spin_op, spin_op_array, this->n, this->q);
        this->basis_ops[i] = spin_op;
        for (int j = 0; j < this->n; ++j){
            this->basis_ops_matrix[j][i] = spin_op_array[j];
        }
    } 
    // Check if the basis in linearly independent
    if (! is_linearly_independent(this->basis_ops_matrix, this->n, this->q)){
        throw std::invalid_argument("Given basis is not linearly independent.");
    }
}

void Basis::set_basis_unsafe(const std::vector<std::vector<uint8_t>>& spin_ops){
    std::vector<__uint128_t> spin_op(this->n_ints, 0);
    std::vector<uint8_t> spin_op_array;
    for (int i = 0; i < spin_ops.size(); i++){
        spin_op_array = spin_ops[i];
        convert_8bit_vec_to_128bit_vec_unsafe(spin_op, spin_op_array, this->n, this->q);
        this->basis_ops[i] = spin_op;
        for (int j = 0; j < this->n; ++j){
            this->basis_ops_matrix[j][i] = spin_op_array[j];
        }
    }      
}

void Basis::set_basis_from_file(std::string& file){
    // Open file
    std::ifstream myfile(file);

    // Check if file exists
    if (myfile.fail()){
        throw std::invalid_argument("Not able to open the file.");
    }

    // Counter for the number of lines/operators
    int n_ops = 0;

    std::string line;
    std::vector<__uint128_t> spin_op(this->n_ints);
    while (getline(myfile, line)){
        // Check if there are at least n characters in a line
        if (line.size() < this->n){
            throw std::invalid_argument("File contains invalid spin operator because it has less than n variables.");
        }
        // Check if there not too many operators given
        if (n_ops == this->n){
            throw std::invalid_argument("File contains too many spin operators.");
        }
        line = line.substr(0, n);
        // Convert from string of n values between 0 and q-1 to a vector of log2(q) 128bit integers
        convert_string_to_vector(spin_op, line, this->n, this->q);
        this->basis_ops[n_ops] = spin_op;

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

void Basis::set_basis_default() {
    // Set the default base: s1, s2, ..., sn
    __uint128_t element = 1;
    for (int i = 0; i < this->n; i++){
        // log2(q) 128 bit integer representation
        this->basis_ops[i][0] = element;
        element <<= 1;
        // n 8bit integer representation
        this->basis_ops_matrix[i][i] = 1;
    }
}

int Basis::set_basis_random() {
    int tries = 0;
    while (true) {
        // Generate random n by n matrix with entries between 0 and q-1
        for (int i = 0; i < this->n; ++i){
            for (int j = 0; j < this->n; ++j){
                this->basis_ops_matrix[i][j] = rand()/(RAND_MAX/this->q);
            }
        }
        ++tries;
        // Repeate until linearly independent
        if (is_linearly_independent(this->basis_ops_matrix, this->n, this->q)){break;}
    }
    return tries;
}

void Basis::print_details() {
    int i = 1;
    std::string op_string;
    for (std::vector<__uint128_t> op : this->basis_ops){
        op_string = convert_128bit_vec_to_string(op, this->n);
        std::cout << "Operator " << i << ": \t" << op_string << "\t Indices: ";
        // Find the indices of the variables present in the spin operator
        for (size_t j = 0; j < this->n; ++j){
            if (op_string[j] != '0') {std::cout << j << "\t";}
        }
        std::cout << "\n";
        ++i;
    }
}