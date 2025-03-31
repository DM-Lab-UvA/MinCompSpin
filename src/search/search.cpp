#include "search/search.h"

/***************
* Constructors *
***************/

MCMSearch::MCMSearch() : mcm_in(1), mcm_out(1) {
    // Set default settings for SA
    this->SA_max_iter = 50000;
    this->SA_T0 = 100;
    this->SA_update_schedule = 100;
}

/*****************
* Public methods *
*****************/

MCM MCMSearch::get_mcm_in() {
    // Check if a search has occured
    if (! this->log_evidence_trajectory.size()){
        throw std::runtime_error("No search has been ran yet.");
    }
    if (this->exhaustive){
        throw std::runtime_error("Exhaustive search does not have an initial MCM.");
    }
    return this->mcm_in;
}

MCM MCMSearch::get_mcm_out() {
    // Check if a search has occured
    if (! this->log_evidence_trajectory.size()){
        throw std::runtime_error("No search has been ran yet.");
    }
    return this->mcm_out;
}

void MCMSearch::set_SA_max_iter(int n_iter) {
    if (n_iter < 1) {
        throw std::invalid_argument("The maximum number of iterations should be a positive number.");
    }
    this->SA_max_iter = n_iter;
}

void MCMSearch::set_SA_init_temp(int temp) {
    if (temp < 1) {
        throw std::invalid_argument("The initial annealing temperature should be positive.");
    }
    this->SA_T0 = temp;
}

void MCMSearch::set_SA_update_schedule(int n_iter) {
    if (n_iter < 1) {
        throw std::invalid_argument("The number of iterations before updating the temperature should be a positive number.");
    }
    this->SA_update_schedule = n_iter;
}

std::vector<double> MCMSearch::get_log_evidence_trajectory(){
    if (! this->log_evidence_trajectory.size()){
        throw std::runtime_error("No search has been ran.");
    }
    return this->log_evidence_trajectory;
}

/******************
* Private methods *
******************/

double MCMSearch::get_log_ev(std::vector<__uint128_t> partition){
    double log_ev = 0;
    // Iterate over all the ICCs in the partition
    for (__uint128_t component : partition){
        // Calculate the evidence of the ICC if it is non-empty
        if (component){
            log_ev += get_log_ev_icc(component);
        }
    }
    return log_ev;
}

double MCMSearch::get_log_ev_icc(__uint128_t component){
    double log_ev;
    // Check if it evidence for this component is already calculated
    if (!this->exhaustive){
        // Not an exhaustive search -> the storage is a map
        std::map<__uint128_t, double>::iterator result = this->evidence_storage.find(component);
        if (result == this->evidence_storage.end()){
            // Not found -> needs to be calculated
            log_ev = this->data->calc_log_ev_icc(component);
            // Store the result
            this->evidence_storage[component] = log_ev;
        }
        else{
            // Retrieve value from storage
            log_ev = this->evidence_storage[component];
        }
    }
    else{
        // Exhaustive search -> Search for value in storage, which is a vector
        log_ev = this->evidence_storage_es[component-1];
        if (log_ev == 0){
            // Not found -> needs to be calculated
            log_ev = this->data->calc_log_ev_icc(component);
            // Store the result
            this->evidence_storage_es[component-1] = log_ev;
        }
    }
    return log_ev;
}