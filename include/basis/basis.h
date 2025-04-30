#pragma once

#include <vector>
#include <fstream>

#include "data/dataset.h"
#include "independence.h"
#include "utilities/spin_ops.h"
#include "utilities/miscellaneous.h"

class Basis {
public:
    /**
     * Constructs a new Basis object with just a number of variables.
     * The default basis is the n one-body operators.
     * 
     * @param n_var                 Number of variables in the system.
     * @param n_states              Number of values each variable can take.     
     */
    Basis(int n_var, int n_states);

    /**
     * Constructs a new Basis object with a specif given basis.
     * 
     * @param n_var                 Number of variables in the system.
     * @param n_states              Number of values each variable can take.
     * @param spin_ops              Vector with spin operators defining the basis.
     */
    Basis(int n_var, int n_states, const std::vector<std::vector<uint8_t>>& spin_ops);

    /**
     * Constructs a new Basis object with a basis specified in a file.
     * 
     * @param n_var                 Number of variables in the system.
     * @param n_states              Number of values each variable can take.
     * @param file                  Path to the file containing the basis.
     */
    Basis(int n_var, int n_states, std::string& file);

    std::vector<std::vector<__uint128_t>> get_basis() {return this->basis_ops;};
    std::vector<std::vector<uint8_t>> get_basis_matrix() {return this->basis_ops_matrix;};
    void set_basis(const std::vector<std::vector<uint8_t>>& spin_ops);
    void set_basis_unsafe(const std::vector<std::vector<uint8_t>>& spin_ops);
    void set_basis_from_file(std::string& file);
    void set_basis_default();
    int set_basis_random();

    void gt_data_in_place(Data& data);
    Data gt_data(const Data& data);

    void gt_basis_in_place(Basis& basis);    

    int get_n() {return this->n;};
    int get_q() {return this->q;};
    int get_n_ints() {return this->n_ints;};

    void print_details();

private:

    int n; // Number of variables in the system
    int q; // Number of states
    int n_ints; // Number of 128bit integers necessary to represent the spin operators

    std::vector<std::vector<__uint128_t>> basis_ops; // Vector of spin operators
    std::vector<std::vector<uint8_t>> basis_ops_matrix; // Matrix representation of the basis 
};
