#include "data/dataset.h"
#include "utilities/histogram.h"

double Data::calc_log_likelihood_icc(__uint128_t component){
    double log_likelihood = 0;
    double N_datapoints = this->N;
    double alpha = this->N_assumed / this->N;
    // Determine the size of the component
    int r = bit_count(component);
    // Get the datapoint frequencies
    std::unordered_map<std::vector<__uint128_t>, unsigned int, HashVector128> counts = build_histogram(*this, component);
    std::unordered_map<std::vector<__uint128_t>, unsigned int, HashVector128>::iterator count_iter = counts.begin();
    while (count_iter != counts.end()){
        log_likelihood += alpha * count_iter->second * log(count_iter->second / N_datapoints);
        ++count_iter;
    }
    return log_likelihood;
}

double Data::calc_log_likelihood(std::vector<__uint128_t>& partition){
    int r = 0;
    double log_likelihood = 0;
    // Iterate over all the ICCs in the partition
    for (__uint128_t component : partition){
        // Calculate the evidence of the ICC if it is non-empty
        if (component){
            log_likelihood += this->calc_log_likelihood_icc(component);
            r += bit_count(component);
        }
    }
    // Contribution from variables not in the model
    log_likelihood -= this->N_assumed * (this->n - r) * log(this->q);
    
    return log_likelihood;
}