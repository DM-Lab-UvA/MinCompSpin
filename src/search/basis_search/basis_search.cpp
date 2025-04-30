#include "search/basis_search/basis_search.h"

/***************
 * Constructor *
 ***************/
BasisSearch::BasisSearch() : basis(1,2) {
    // Initially no search is run
    this->search_method = "None";
}

/******************
 * Search methods *
 ******************/

Basis BasisSearch::up_to_order_k(const Data& data, int k, const std::string file_name){
    // Timing
    auto start = std::chrono::system_clock::now();

    // Create matrix to store operators (n by n_ops)
    std::vector<std::vector<uint8_t>> spin_ops_matrix;
    sort_spin_ops(data, spin_ops_matrix, k);

    // Find the first n most biased independent operators using Gaussian elimination (add mod q) of the matrix
    std::vector<uint8_t> columns = row_reduce(spin_ops_matrix, data.q);

    // Create Basis object with the found spin operators
    int i = 0;
    std::vector<std::vector<uint8_t>> spin_ops(data.n, std::vector<uint8_t>(data.n));
    for (uint8_t index : columns){
        for (int j = 0; j < data.n; ++j){
            spin_ops[i][j] = spin_ops_matrix[j][index];
        }
        ++i;
    }
    Basis opt_basis(data.n, data.q, spin_ops);

    // Timing
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // Store the results
    if (k == data.n){
        this->search_method = "exhaustive";
    }
    else {
        this->search_method = "up_to_order_k";
    }
    this->max_order = k;
    this->basis = opt_basis;
    this->duration = elapsed.count();

    // Write basis to file
    if (!file_name.empty()){
        // Create the output file
        std::ofstream output_file(file_name);
        if (!output_file){
            std::cerr << "Error: Could not open the output file." << std::endl;
        }
        for (std::vector<__uint128_t> op : opt_basis.get_basis()){
            output_file << convert_128bit_vec_to_string(op, data.n) << '\n';
        }
    }

    return opt_basis;
}

Basis BasisSearch::iterative_search(const Data& data, int k, const std::string file_name){
    // Timing
    auto start = std::chrono::system_clock::now();

    // Make a copy to transform in place during the interations
    Data data_copy = data;

    // First transfromation
    Basis basis_ri = this->up_to_order_k(data_copy, k);
    // Variable to keep track of the basis relative to the original variables
    Basis basis_r0 = basis_ri;

    bool identity = is_identity(basis_ri.get_basis_matrix(), data.n);

    int n_iter = 0;
    while (! identity){
        // Transform data into the new basis
        basis_ri.gt_data_in_place(data_copy);
        // Find the new best basis up to order k
        basis_ri = this->up_to_order_k(data_copy, k);
        identity = is_identity(basis_ri.get_basis_matrix(), data.n);
        ++n_iter;
        if (identity){break;}
        // Transform to know the new basis in terms of the original variables
        basis_ri.gt_basis_in_place(basis_r0);
    }
    // Timing
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // Store the results
    this->search_method = "iterative";
    this->n_iterations = n_iter;
    this->max_order = k;
    this->basis = basis_r0;
    this->duration = elapsed.count();

    // Write basis to file
    if (!file_name.empty()){
        // Create the output file
        std::ofstream output_file(file_name);
        if (!output_file){
            std::cerr << "Error: Could not open the output file." << std::endl;
        }
        for (std::vector<__uint128_t> op : basis_r0.get_basis()){
            output_file << convert_128bit_vec_to_string(op, data.n) << '\n';
        }
    }

    return basis_r0;
}

bool is_identity(const std::vector<std::vector<uint8_t>>& matrix, int n){
    // Check if only the diagonal elements are non-zero
    for (int i = 0; i < n; ++i){
        for (int j = 0; j < n; ++j){
            if (i == j){
                if (!matrix[i][j]){return false;}
            }
            else{
                if (matrix[i][j]){return false;}
            }
        }
    }
    return true;
}

/******************
 * Search results *
 ******************/

Basis BasisSearch::get_basis() {
    // Only return if search has been ran
    if (this->search_method == "None"){
        throw std::runtime_error("No search has been ran yet.");
    }
    return this->basis;
}

void BasisSearch::print_search_results() {
    // Only if search has been ran
    if (this->search_method == "None"){
        throw std::runtime_error("No search has been ran yet.");
    }

    std::cout << "Search method: " << this->search_method << "\n \n";
    std::cout << "Duration: " << this->duration << "s \n";
    if (search_method == "iterative"){
        std::cout << "Number of iterations: " << this->n_iterations << "\n";
    }
    if (search_method != "exhaustive"){
        std::cout << "Maximum interaction order: " << this->max_order << "\n";
    }
    std::cout << "\n";
    // Print basis
    this->basis.print_details();
}


