#pragma once

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <map>

#include "data_processing.h"

class Data {
public:
    /**
     * Constructs a new Data object.
     * 
     * @param file                  Path to the file.
     * @param n_var                 Number of variables in the system.
     * @param n_states              Number of values each variable can take.
     */
    Data(const std::string& filename, int n_var, int n_states);

    /**
     * Calculates the entropy of the dataset.
     * 
     * @param base                  Base of the logarithm used (default is 2).
     * 
     * @return entropy              The entropy of the dataset as a double.
     */
    double entropy(int base = -1);

    /**
     * Calculates the log evidence of a given component.
     * 
     * @param component             Integer representation of the bitstring representing a component.
     * 
     * @return log_ev               The log evidence of the component as a double. 
     */
    double calc_log_ev_icc(__uint128_t component);

    /**
     * Calculates the log evidence of a given partition.
     * 
     * @param partition             Partition as a vector of n integers representing the components.
     * 
     * @return log_ev               The log evidence of the partition as a double. 
     */    
    double calc_log_ev(std::vector<__uint128_t>& partition);

    std::vector<std::pair<std::vector<__uint128_t>, unsigned int>> dataset;

    int n; // Number of variables
    int q; // Number of states
    int N; // Number of datapoints
    int N_unique; // Number of different datapoints
    int n_ints; // Number of 128bit integers necessary to represent the data

    std::vector<__uint128_t> pow_q; // Vector containing the first n powers of q used to speed up the calculation of the evidence
};
