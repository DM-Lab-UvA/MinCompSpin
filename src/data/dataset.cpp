#include "data/dataset.h"
#include "utilities/histogram.h"

Data::Data(const std::string& filename, int n_var, int n_states){
    // Check if the given number of variables is valid
    if (n_var > 128){
        throw std::invalid_argument("The maximum system size is 128 variables.");
    }
    if (n_var < 1){
        throw std::invalid_argument("The system size should be at least 1.");
    }
    // Assign variables
    this->n = n_var;
    this->q = n_states;

    // Calculate the number of integers necessary to represent the data
    this->n_ints = ceil(log2(q));
    // Process the dataset
    this->N = processing(filename, n_var, n_ints, n_states, dataset);
    this->N_assumed = this->N;
    this->N_unique = dataset.size();

    // Precompute the powers of q to speed up the calculation of the evidence (q^r)
    this->pow_q.assign(n+1, 0);
    __uint128_t element = 1;
    for(int i = 0; i < n+1; i++){
        this->pow_q[i] = element;
        element *= q;
    }
}

Data::Data(const std::vector<std::pair<std::vector<__uint128_t>, unsigned int>>& _dataset, int n_var, int n_states, int n_samples){
    // Assign variables
    this->n = n_var;
    this->q = n_states;
    this->dataset = _dataset;
    this->N = n_samples;
    this->N_assumed = this->N;
    this->N_unique = _dataset.size();

    // Calculate the number of integers necessary to represent the data
    this->n_ints = ceil(log2(q));   
    
    // Precompute the powers of q to speed up the calculation of the evidence (q^r)
    this->pow_q.assign(n+1, 0);
    __uint128_t element = 1;
    for(int i = 0; i < n+1; i++){
        this->pow_q[i] = element;
        element *= q;
    }
}

void Data::set_N_assumed(int n_datapoints){
    // Check if the given number of datapoints is valid
    if (n_datapoints < 1){
        throw std::invalid_argument("The number of datapoints should be a positive number.");
    }
    this->N_assumed = n_datapoints;
}

double Data::entropy(int base){
    // Default base is q
    if (base == -1){
        base = this->q;
    }
    // Check if the base is a valid integer
    else if (base < 2){
        throw std::invalid_argument("Base should be a positive integer larger than or equal to 2.");
    }
    
    // Determine the frequencies of all the different states
    std::unordered_map<std::vector<__uint128_t>, unsigned int, HashVector128> counts = build_histogram(*this);
    std::unordered_map<std::vector<__uint128_t>, unsigned int, HashVector128>::iterator count_iter = counts.begin();

    double entropy = 0;
    double p;

    // Convert the number of datapoints to a double such that p isn't rounded to the nearest integer
    double N = this->N;

    while (count_iter != counts.end()){
        p = count_iter->second / N;
        entropy -= p * log(p) / log(base);
        ++count_iter;
    }
    return entropy;
}
