#include "utilities/partition.h"
#include "utilities/miscellaneous.h"

/*
std::vector<int> generate_random_partition(int n){
    // Check if the number of variables is a valid number
    if (n > 128){
        throw std::domain_error("The maximum system size is 128 variables.");
    }
    if (n < 0){
        throw std::domain_error("The number of variables should be a positive number.");
    }

    std::vector<int> growth_string(n, 0);

    int index;
    int max_index = 2;
    for (int i = 1; i < n; i++){
        index = rand()/(RAND_MAX/max_index);
        growth_string[i] = index;

        if (index == (max_index-1)){
            max_index += 1;
        }
    }

    return growth_string;
}*/

std::vector<__uint128_t> generate_random_partition(int n){
    // Check if the number of variables is a valid number
    if (n > 128){
        throw std::domain_error("The maximum system size is 128 variables.");
    }
    if (n < 0){
        throw std::domain_error("The number of variables should be a positive number.");
    }

    int component;
    __uint128_t element = 1;
    std::vector<__uint128_t> partition(n, 0);
    for (int i = 0; i < n; i++){
        // Generate a random number between 0 and n-1
        component = rand()/(RAND_MAX/n);
        // Add variable i to this component
        partition[component] += element;
        element <<= 1;
    }
    return partition;
}

std::vector<__uint128_t> generate_independent_partition(int n){
    // Check if the number of variables is a valid number
    if (n > 128){
        throw std::domain_error("The maximum system size is 128 variables.");
    }
    if (n < 0){
        throw std::domain_error("The number of variables should be a positive number.");
    }
    __uint128_t element = 1;
    std::vector<__uint128_t> partition(n, 0);
    for (int i = 0; i < n; i++){
        // Add variable i to this component
        partition[i] += element;
        element <<= 1;
    }
    return partition;
}

std::vector<__uint128_t> generate_complete_partition(int n){
    // Check if the number of variables is a valid number
    if (n > 128){
        throw std::domain_error("The maximum system size is 128 variables.");
    }
    if (n < 0){
        throw std::domain_error("The number of variables should be a positive number.");
    }
    __uint128_t element = 1;
    std::vector<__uint128_t> partition(n, 0);
    for (int i = 0; i < n; i++){
        // Add variable i to this component
        partition[0] += element;
        element <<= 1;
    }
    return partition;
}

void convert_partition(int* a, std::vector<__uint128_t>& partition, int n){
    __uint128_t element = 1;
    // Loop over all variables
    for (int i = 0; i < n; ++i){
            // The element a[i] is the index of the component to which variable i belongs
            partition[a[i]] += element;
            // Bitshift to the left to get the integer value of the next variable
            element <<= 1;
        }
}

void print_partition_to_file(std::ofstream& file, std::vector<__uint128_t>& partition){
    // Number of variables
    int n = partition.size();
    // Counter for the component number
    int i = 0;
    for (__uint128_t component : partition){
        // Ignore empty component
        if (! component){continue;}
        file << "Component " << i << " : \t" << int_to_string(component, n) << '\n';
        ++i;
    }
}

void print_partition_details_to_file(std::ofstream& file, MCM& mcm, int N, int q){
    for (int i = 0; i < mcm.n_comp; ++i){
        file << "Component " << i << " : \t" << int_to_string(mcm.partition[i], mcm.n) << "\t Size: " << bit_count(mcm.partition[i]) << "\t Log-evidence (q-its/datapoint): " << mcm.log_ev_per_icc[i] / (N * log(q)) << '\n';
    }
}

void print_partition_to_terminal(std::vector<__uint128_t>& partition){
    // Number of variables
    int n = partition.size();
    // Counter for the component number
    int i = 0;
    for (__uint128_t component : partition){
        // Ignore empty component
        if (! component){continue;}
        std::cout << "Component " << i << " : \t" << int_to_string(component, n) << '\n';
        ++i;
    }
    std::cout << '\n';
}
