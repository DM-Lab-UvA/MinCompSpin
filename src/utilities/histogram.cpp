#include "utilities/histogram.h"

std::unordered_map<std::vector<__uint128_t>, unsigned int, HashVector128> build_histogram(Data& data, __uint128_t component){
    // Map to keep track of the frequency of the datapoints
    std::unordered_map<std::vector<__uint128_t>, unsigned int, HashVector128> counts;
    std::vector<__uint128_t> state(data.n_ints);
    // Loop over the entire dataset
    for (auto const &it : data.dataset){
        // Bitwise AND to extract the substring corresponding to the component
        for (int i = 0; i < data.n_ints; ++i){
            state[i] = it.first[i] & component;
        }
        // Increase frequency of the state
        counts[state] += it.second;   
    }
    return counts;
}

std::unordered_map<std::vector<__uint128_t>, unsigned int, HashVector128> build_histogram(Data& data){
    // Map to keep track of the frequency of the datapoints
    std::unordered_map<std::vector<__uint128_t>, unsigned int, HashVector128> counts;
    std::vector<__uint128_t> state(data.n_ints);
    // Loop over the entire dataset
    for (auto const &it : data.dataset){
        // Increase frequency of the state
        counts[it.first] += it.second;   
    }
    return counts;
}
