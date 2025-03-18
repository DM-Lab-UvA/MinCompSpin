#pragma once

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <map>

#include "utilities/miscellaneous.h"

/**
 * Reads in and processes the dataset.
 * 
 * @param file                  Path to the file.
 * @param n                     Number of variables in the system.
 * @param n_ints                Number of 128bit integers necessary to represent the data.
 * @param n_states              Number of states.
 * @param data                  Empty vector to store the dataset in.
 * 
 * @return N                    The number of datapoints.
 */
int processing(std::string file, int n, int n_ints, int n_states, std::vector<std::pair<std::vector<__uint128_t>, unsigned int>>& data);

/**
 * Converts the data from a string to log2(q) 128bit integers.
 * 
 * @param[in, out] obs          Vector of log2(q) 128bit integers that will contain the converted observation.
 * @param[in] raw_obs           The observation as a string of length n.
 * @param n                     Number of variables in the system.
 * @param q                     Number of states.
 * 
 * @return void                 Nothing is returned by this function.
 */
void convert_observation(std::vector<__uint128_t>& obs, std::string& raw_obs, int n, int q);
