#include "data/data_processing.h"

int processing(std::string file, int n, int n_ints, int n_states, std::vector<std::pair<std::vector<__uint128_t>, unsigned int>>& data){
    // Open file
    std::ifstream myfile(file);

    // Store dataset as a map of vectors containing n_ints 128bit integers
    std::unordered_map<std::vector<__uint128_t>, unsigned int, HashVector128> dataset;

    // Check if file exists
    if (myfile.fail()){
        throw std::invalid_argument("Not able to open the file.");
    }
    // Counter for the number of datapoints
    int N = 0;

    std::string line;
    std::vector<__uint128_t> observation(n_ints);
    while (getline(myfile, line)) {
        // Check if there are at least n variables in the observation
        if (line.size() < n){
            throw std::invalid_argument("File contains datapoints with less than n variables.");
        }
        // Exctract the first n variables from the observation
        line = line.substr(0, n);
        // Convert from string of n values between 0 and q-1 to a vector of log2(q) 128bit integers
        convert_string_to_vector(observation, line, n, n_states);
        dataset[observation]++;
        N++;
    }

    // Store the pairs in a vector
    for (auto &my_pair : dataset) {
        data.push_back(my_pair);
    }
    return N;
}