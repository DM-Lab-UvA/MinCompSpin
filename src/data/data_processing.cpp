#include "data/data_processing.h"

int processing(std::string file, int n, int n_ints, int n_states, std::vector<std::pair<std::vector<__uint128_t>, unsigned int>>& data){
    // Open file
    std::ifstream myfile(file);

    // Store dataset as a map of vectors containing n_ints 128bit integers
    std::map<std::vector<__uint128_t>, unsigned int> dataset;

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
        // Convert the observation and add it to the dataset
        convert_observation(observation, line, n, n_states);
        dataset[observation]++;
        N++;
    }

    // Store the pairs in a vector
    for (auto &my_pair : dataset) {
        data.push_back(my_pair);
    }
    return N;
}

void convert_observation(std::vector<__uint128_t>& obs, std::string& raw_obs, int n, int q){
    __uint128_t ONE = 1;
    // Set all elements equal to zero
    std::fill(obs.begin(), obs.end(), 0);
    // Variable for the integer value of the ith bit
    __uint128_t element = 1;
    // Loop over the variables
    for (int i = 0; i < n; ++i){
        // Convert value of variable i from string to integer
        int value = raw_obs[i] - '0';
        // Check if the value is between 0 and q-1
        if (value > q - 1) {
            throw std::invalid_argument("The dataset should only contain values between 0 and q-1.");
        }
        int bit = 0;
        while (value){
            // Check if last bit in the binary representation is nonzero
            if (value & 1){
                obs[bit] += element;
            }
            // Bitshift to the right
            ++bit;
            value >>= 1;
        }
        // Bitshift to the left to get the decimal value of the next variable
        element <<= 1;
    }
}