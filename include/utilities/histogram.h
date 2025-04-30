#pragma once

#include "data/dataset.h"

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
