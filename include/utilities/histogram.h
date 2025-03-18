#pragma once

#include "data/dataset.h"

/**
 * Hash function for a vector containing 128bit integers
 * 
 * @struct HashVector128
 */
struct HashVector128 {
    std::size_t operator()(const std::vector<__uint128_t>& vec) const {
        std::size_t seed = 0;
        for (const __uint128_t& val : vec) {
            // Combine the hashes of each element
            seed ^= val + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

/**
 * Counts all the different observations in the dataset for a given component.
 * 
 * @param data                  Data object containing the characteristic of the dataset. 
 * @param component             Integer representation of the bitstring representing a component.
 * 
 * @return Distribution of the states that occur as a map.   
 */
std::unordered_map<std::vector<__uint128_t>, unsigned int, HashVector128> build_histogram(Data& data, __uint128_t component);

/**
 * Counts all the different observations in the dataset.
 * 
 * @param data                  Data object containing the characteristic of the dataset. 
 * 
 * @return Distribution of the states that occur as a map.   
 */
std::unordered_map<std::vector<__uint128_t>, unsigned int, HashVector128> build_histogram(Data& data);
