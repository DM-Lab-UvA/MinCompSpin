#pragma once

#include <stdio.h>
#include <float.h>
#include <vector>
#include <random>

#include "data/dataset.h"
#include "utilities/miscellaneous.h"
#include "utilities/partition.h"


class MCM {
public:
    /**
     * Constructs a new MCM object with just a number of variables.
     * Default partition is the independent model.
     * 
     * @param n                     Number of variables in the system.
     */
    MCM(int n);

    /**
     * Constructs a new MCM object with the number of variables and a specific partition.
     * 
     * @param n                     Number of variables in the system.
     * @param partition             Partition as a vector of n integers representing the components.
     */
    MCM(int n, std::vector<__uint128_t> partition);

    /**
     * Constructs a new MCM object with the number of variables and a partition.
     * 
     * @param n                     Number of variables in the system.
     * @param partition             Partition as string. Valid options are:
     *                                  -"independent": all variables in a separate component
     *                                  -"complete": all variables in a single component
     *                                  -"random": each variable is assigned to a random component
     */
    MCM(int n, std::string partition);
    
    /**
     * Returns the current partition
     * 
     * @return partition            Partition as a vector of n integers representing the components.
     */
    std::vector<__uint128_t>& get_partition() {return this->partition;};

    /**
     * Set the partition of the mcm. Note that this will overwrite the solution if a search has been ran before
     * 
     * @param partition             Partition as a vector of n integers representing the components.
     */
    void set_starting_partition(std::vector<__uint128_t>& partition);

    /**
     * Returns the best log evidence of the partition. 
     * Works only after a search for the best mcm has been ran for a given dataset.
     * 
     * @return log_ev               Log evidence of the partition for a given dataset as a double.
     */
    double get_best_log_ev();

    /**
     * Returns the best log evidence of the partition per component. 
     * Works only after a search for the best mcm has been ran for a given dataset.
     * 
     * @return log_ev               Log evidence of the partition per ICC for a given dataset as a double.
     */  
    std::vector<double>& get_best_log_ev_per_icc();

    /**
     * Prints out the information about the partition and the log-evidence of the MCM.
     * Works only after a search for the best mcm has been ran for a given dataset.
     */
    void print_info();

    /**
     * Generate N samples from this MCM combined with a given dataset.
     * The N samples are written to a given file.
     * 
     * @param N                     The number of samples that need to be generated.
     * @param data                  The dataset from which the model parameters are inferred.
     * @param file_name             Path to the file where that will contain the generated data.
     */
    void generate_data_file(int N, const Data& data, const std::string& file_name);

    /**
     * Generate N samples from this MCM combined with a given dataset.
     * The N samples are returned as a Data object and written to a given file.
     * 
     * @param N                     The number of samples that need to be generated.
     * @param data                  The dataset from which the model parameters are inferred.
     * 
     * @return samples              Data object containing the generated samples as the dataset.
     */
    Data generate_data_object(int N, const Data& data);

    int n; // Number of variables present in the system
    int n_comp; // Number of non-empty components in the partition
    int rank; // Number of variables present in the partition
    std::vector<__uint128_t> partition;

    // Information about the evidence of the mcm for a given dataset after search has been ran
    double log_ev;
    std::vector<double> log_ev_per_icc;

    bool optimized; // Indicates if a search for the best mcm has been ran
};