#pragma once

#include <vector>

#include "utilities/miscellaneous.h"

/**
 * Struct containing the parameter settings of the simulated annealing algorithm
 * 
 * @struct SA_settings
 * 
 * @var SA_settings::temp
 *  Double indicating the current temperature.
 * 
 * @var SA_settings::epsilon
 *  Double indicating the precision that is used when comparing the log evidence of two partitions.
 * 
 * @var SA_settings::max_no_improve
 *  Integer indicating the number of iterations without improvement after which the algorithm stops.
 * 
 * @var SA_settings::occupied_comp
 *  128bit integer for which the corresponding binary string has the ith bit set to 1 if the ith component is non-empty and zero otherwise.
 * 
 * @var SA_settings::occupied_comp2
 *  128bit integer for which the corresponding binary string has the ith bit set to 1 if the ith component contains at least 2 variables and zero otherwise. 
 */
struct SA_settings {
    double temp;
    double epsilon = 1e-4;
    int max_no_improve = 10000;
    __uint128_t occupied_comp = 0;
    __uint128_t occupied_comp2 = 0;

    /**
     * Constructs a SA_settings struct
     * 
     * @param T0            Initial temperature.
     * @param partition     Starting partition.
     */
    SA_settings(int T0, std::vector<__uint128_t>& partition) {
        this->temp = T0;
        
        int r;
        int n = partition.size();
        __uint128_t element = 1;
        for (int i = 0; i < n; i++){
            r = bit_count(partition[i]);
            if (r > 0){
                this->occupied_comp += element;
            }
            if (r > 1){
                this->occupied_comp2 += element;
            }
            element <<= 1;
        }
    }
};