#include "data/dataset.h"
#include "utilities/histogram.h"

double Data::calc_log_ev_icc(__uint128_t component){
    double log_evidence = 0;
    // Determine the size of the component
    int r = bit_count(component);
    double alpha = this->N_assumed / this->N;
    // Contributions from the datapoint frequencies
    std::unordered_map<std::vector<__uint128_t>, unsigned int, HashVector128> counts = build_histogram(*this, component);
    std::unordered_map<std::vector<__uint128_t>, unsigned int, HashVector128>::iterator count_iter = counts.begin();
    while (count_iter != counts.end()){
        log_evidence += (lgamma(alpha * count_iter->second + 0.5) - 0.5 * log(M_PI));
        ++count_iter;
    }

    // Calculate prefactor
    if (r > 25){
        // Approximate for large components because lgamma overflows
        log_evidence -= (r * log(this->q) * this->N_assumed);
    }
    else{
        log_evidence += lgamma(this->pow_q[r]/2.) - lgamma(this->N_assumed + pow_q[r]/2.);
    }
    
    return log_evidence;
}

double Data::calc_log_ev(std::vector<__uint128_t>& partition){
    int r = 0;
    double log_ev = 0;
    // Iterate over all the ICCs in the partition
    for (__uint128_t component : partition){
        // Calculate the evidence of the ICC if it is non-empty
        if (component){
            log_ev += this->calc_log_ev_icc(component);
            r += bit_count(component);
        }
    }
    // Contribution from variables not in the model
    log_ev -= this->N_assumed * (this->n - r) * log(this->q);

    return log_ev;
}
