#include "model/mcm.h"

MCM::MCM(int n) {
    // Check if the given number of variables is valid
    if (n < 1){
        throw std::invalid_argument("The number of variables should be a non-zero positive number.");
    }
    if (n > 128){
        throw std::invalid_argument("The maximum system size is 128 variables.");
    }
    // Default partition: independent model
    this->partition = generate_independent_partition(n);
    // Set variable
    this->n = n;
    this->n_comp = n;
    this->rank = n;
    // Initially no search is run
    this->optimized = false;
    this->log_ev = -DBL_MAX;
}

MCM::MCM(int n, std::vector<__uint128_t> partition){
    // Check if the given number of variables is valid
    if (n < 1){
        throw std::invalid_argument("The number of variables should be a non-zero positive number.");
    }
    if (n > 128){
        throw std::invalid_argument("The maximum system size is 128 variables.");
    }
    // Set variable
    this->n = n;
    this->rank = 0;

    this->partition.assign(n, 0);
    this->n_comp = 0;
    __uint128_t all_vars = ((__uint128_t)1 << n) -1;
    __uint128_t assigned_vars = 0;
    int partition_size = partition.size();
    for (int i = 0; i < partition_size; i++){
        // Check if the component is non-empty
        if(partition[i]){
            // Check if variable has not been assigned before
            if (assigned_vars & partition[i]){
                throw std::invalid_argument("Invalid partition because the same variable occurs in multiple components.");
            }
            // Check if the given partition does more than n variables
            assigned_vars += partition[i];
            if (assigned_vars > all_vars){
                throw std::invalid_argument("Invalid partition because there are more than n variables.");
            }
            // Copy the component
            this->partition[this->n_comp] = partition[i];
            
            // Update the number of non-empty components and number of variables in the partition
            this->n_comp++;
            this->rank += bit_count(partition[i]);
        }
    }
    // Initially no search is run
    this->optimized = false;
    this->log_ev = -DBL_MAX;
}

MCM::MCM(int n, std::string partition){
    // Check if the given number of variables is valid
    if (n < 1){
        throw std::invalid_argument("The number of variables should be a non-zero positive number.");
    }
    if (n > 128){
        throw std::invalid_argument("The maximum system size is 128 variables.");
    }
    // Set variable
    this->n = n;
    this->rank = n;
    if (partition == "independent"){
        this->partition = generate_independent_partition(n);
        this->n_comp = n;
    }
    else if (partition == "complete"){
        this->partition = generate_complete_partition(n);
        this->n_comp = 1;
    }
    else if (partition == "random"){
        this->partition = generate_random_partition(n);
        // Count the number of non-empty components
        this->n_comp = 0;
        for(int i = 0; i < n; i++){
            if (this->partition[i]){this->n_comp++;}
        }
        place_empty_entries_last(this->partition);
    }
    else {
        throw std::invalid_argument("Invalid partition type. Options are 'independent', 'complete' or 'random'.");
    }
    // Initially no search is run
    this->optimized = false;
    this->log_ev = -DBL_MAX;
}

void MCM::set_starting_partition(std::vector<__uint128_t>& partition){
    // Check if the partition size is valid
    if (partition.size() > this->n){
        throw std::invalid_argument("The given partition contains more than n components.");
    }
    __uint128_t ONE = 1;

    // Start from a partition with n empty components
    this->n_comp = 0;
    this->rank = 0;
    if (! this->partition.size()){
        this->partition.assign(n,0);
    }
    else {
        std::fill(this->partition.begin(), this->partition.end(), 0);
    }
    
    __uint128_t all_vars = (1 << this->n) -1;
    __uint128_t assigned_vars = 0;
    for (int i = 0; i < this->n; i++){
        // Check if the component is non-empty
        if(partition[i]){
            // Check if variable has not been assigned before
            if (assigned_vars & partition[i]){
                throw std::invalid_argument("Invalid partition because the same variable occurs in multiple components.");
            }
            // Check if the given partition does more than n variables
            assigned_vars += partition[i];
            if (assigned_vars > all_vars){
                throw std::invalid_argument("Invalid partition because there are more than n variables.");
            }
            // Copy the component
            this->partition[this->n_comp] = partition[i];
            // Update the number of non-empty components and number of variables in the partition
            this->n_comp++;
            this->rank += bit_count(partition[i]);
        }
    }

    // Reset if a search has been ran before
    if (this->optimized){
        this->optimized = false;
        this->log_ev_per_icc.clear();
    } 
}

double MCM::get_best_log_ev(){
    // Check if a search for the best MCM has been done
    if (!this->optimized){
        throw std::runtime_error("No search has been ran yet.");
    }
    return this->log_ev;
}

std::vector<double>& MCM::get_best_log_ev_per_icc(){
    // Check if a search for the best MCM has been done
    if (!this->optimized){
        throw std::runtime_error("No search has been ran yet.");
    }
    return this->log_ev_per_icc;
}

void MCM::print_info(){
    // Check if a search for the best MCM has been done
    if (!this->optimized){
        throw std::runtime_error("No search has been ran yet.");
    }
    std::cout << "MCM contains " << this->rank << " variables divided into " << this->n_comp << " components" << std::endl;
    std::cout << "Total log-evidence: " << this->log_ev << std::endl;

    for (int i = 0; i < this->n_comp; ++i){
        std::cout << "Component " << i << " : \t" << int_to_string(this->partition[i], n) << "\t Size: " << bit_count(this->partition[i]) << "\t Log-evidence: " << this->log_ev_per_icc[i] << '\n';
    }
}

void MCM::generate_samples(int N, const Data& data, const std::string& file_name){
    // Check the number of variables
    if (this->n != data.n) {
        throw std::invalid_argument("Number of variables in the MCM doesn't match the number of variables in the given dataset.");
    }
    
    // Create the output file
    std::ofstream output_file(file_name);
    if (!output_file){
        std::cerr << "Error: Could not open the output file." << std::endl;
    }

    // Create a random device and use it to generate a random seed
    std::random_device myRandomDevice;
    unsigned seed = myRandomDevice();

    // Initialize a default_random_engine with the seed
    std::default_random_engine generator(seed);

    // Build a distribution of the states in the dataset
    std::vector<int> weights(data.N_unique);

    int i = 0;
    for (std::pair<std::vector<__uint128_t>, unsigned int> const& datapoint : data.dataset)
    {
        weights[i] = datapoint.second;
        i++;
    }
    std::discrete_distribution<int> distribution(weights.begin(), weights.end());

    int state_index;
    std::vector<__uint128_t> sample_tmp(data.n_ints);
    std::vector<__uint128_t> sample(data.n_ints, 0);
    for (int i = 0; i < N; ++i){
        for (__uint128_t comp : this->partition){
            // Generate datapoint from the distribution
            state_index = distribution(generator);
            sample_tmp = data.dataset[state_index].first;

            // Extract the part of the datapoint that corresponds to the current component
            for (int j = 0; j < data.n_ints; ++j){
                sample[j] += sample_tmp[j] & comp;
            }

        }
        // Write the sample to the output file
        output_file << convert_128bit_vec_to_string(sample, data.n) << '\n';
        std::fill(sample.begin(), sample.end(), 0);
    }
    output_file.close();
}

Data MCM::generate_data(int N, const Data& data, const std::string& file_name){
    // Generate the samples
    this->generate_samples(N, data, file_name);
    // Create data object from the generated file
    Data samples = Data(file_name, data.n, data.q);

    return samples;
}