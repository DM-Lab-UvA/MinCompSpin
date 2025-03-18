#pragma once

#include <stdio.h>
#include <float.h>
#include <vector>

#include "utilities/miscellaneous.h"
#include "utilities/partition.h"

class Model {
public:
    /**
     * Constructs a new Model object with just a number of variables.
     * 
     * @param n                     Number of variables in the system.
     */
    Model(int n);

    /**
     * Constructs a new Model object with the number of variables and a specific partition.
     * 
     * @param n                     Number of variables in the system.
     * @param partition             Partition as a vector of n integers representing the components.
     */
    Model(int n, std::vector<__uint128_t> partition);

    /**
     * Constructs a new Model object with the number of variables and a partition.
     * 
     * @param n                     Number of variables in the system.
     * @param partition             Partition as string. Valid options are:
     *                                  -"independent": all variables in a separate component
     *                                  -"complete": all variables in a single component
     *                                  -"random": each variable is assigned to a random component
     */
    Model(int n, std::string partition);
    
    /**
     * Returns the current partition
     * 
     * @return partition            Partition as a vector of n integers representing the components.
     */
    std::vector<__uint128_t>& get_partition() {return this->partition;};

    /**
     * Set the partition of the model. Note that this will overwrite the solution if a search has been ran before
     * 
     * @param partition             Partition as a vector of n integers representing the components.
     */
    void set_starting_partition(std::vector<__uint128_t>& partition);

    /**
     * Returns the best log evidence of the partition. 
     * Works only after a search for the best model has been ran for a given dataset.
     * 
     * @return log_ev               Log evidence of the partition for a given dataset as a double.
     */
    double get_best_log_ev();

    /**
     * Returns the best log evidence of the partition per component. 
     * Works only after a search for the best model has been ran for a given dataset.
     * 
     * @return log_ev               Log evidence of the partition per ICC for a given dataset as a double.
     */  
    std::vector<double>& get_best_log_ev_per_icc();

    int n; // Number of variables present in the system
    int n_comp; // Number of non-empty components in the partition
    int rank; // Number of variables present in the partition
    std::vector<__uint128_t> partition;

    // Information about the evidence of the model for a given dataset after search has been ran
    double log_ev;
    std::vector<double> log_ev_per_icc;

    bool optimized; // Indicates if a search for the best model has been ran
};