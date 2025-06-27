#include "data/dataset.h"

double Data::calc_geom_complexity_icc(__uint128_t component){
    // Determine the size of the component
    int r = bit_count(component);
    return (this->pow_q[r] / 2.) * log(M_PI) - lgamma(this->pow_q[r]/2.);
}

double Data::calc_geom_complexity(std::vector<__uint128_t>& partition){
    double geom_complexity = 0;
    // Iterate over all the ICCs in the partition
    for (__uint128_t component : partition){
        // Calculate the geometric complexity of the ICC if it is non-empty
        if (component){
            geom_complexity += this->calc_geom_complexity_icc(component);
        }
    }
    return geom_complexity;
}

double Data::calc_param_complexity_icc(__uint128_t component){
    // Determine the size of the component
    int r = bit_count(component);
    __uint128_t n_param = this->pow_q[r] - 1;
    return (n_param / 2.) * log(this->N_assumed / (2 * M_PI));
}

double Data::calc_param_complexity(std::vector<__uint128_t>& partition){
    double param_complexity = 0;
    // Iterate over all the ICCs in the partition
    for (__uint128_t component : partition){
        // Calculate the parametric complexity of the ICC if it is non-empty
        if (component){
            param_complexity += this->calc_param_complexity_icc(component);
        }
    }
    return param_complexity;    
}

double Data::calc_mdl(std::vector<__uint128_t>& partition){
    double mdl = 0;
    // Log likelihood contribution
    mdl = this->calc_log_likelihood(partition);

    // Contribution from the model complexity
    mdl -= this->calc_geom_complexity(partition);
    mdl -= this->calc_param_complexity(partition);
    
    return mdl;
}