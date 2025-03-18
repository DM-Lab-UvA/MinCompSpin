#pragma once

#include <iostream>
#include <fstream>
#include <vector>

/**
 * Generate a random partition of n variables.
 * 
 * @param n                     Number of variables.
 * 
 * @return partition            Random partition of the variables as a vector of n 128 bit integers.
 */
//std::vector<int> generate_random_partition(int n);

/**
 * Generate a random partition of n variables.
 * 
 * @param n                     Number of variables.
 * 
 * @return partition            Random partition of the variables as a vector of n 128 bit integers.
 */
std::vector<__uint128_t> generate_random_partition(int n);

/**
 * Generate the independent partition of n variables.
 * 
 * @param n                     Number of variables.
 * 
 * @return partition            The independent partition of the variables as a vector of n 128 bit integers.
 */
std::vector<__uint128_t> generate_independent_partition(int n);

/**
 * Generate the complete partition of n variables.
 * 
 * @param n                     Number of variables.
 * 
 * @return partition            The complete partition of the variables as a vector of n 128 bit integers.
 */
std::vector<__uint128_t> generate_complete_partition(int n);


void place_empty_component_last(std::vector<__uint128_t>& partition);



/**
 * Converts the partition from a restricted growth string to a vector of n integers representing the components.
 * 
 * @param a                     Array of size n that represents the partition as a restricted growth string.
 * @param partition             Partition as a vector of n integers representing the components.
 * @param n                     Number of variables in the system.
 * 
 * @return void                 Nothing is returned by this function.
 */
void convert_partition(int* a, std::vector<__uint128_t>& partition, int n);

/**
 * Prints the partition as bitstrings of the non-empty components to a file.
 * 
 * @param file                  Path to the file where the partition should be written.
 * @param partition             Partition as a vector of n integers representing the components.
 * 
 * @return void                 Nothing is returned by this function.
 */
void print_partition_to_file(std::ofstream& file, std::vector<__uint128_t>& partition);

/**
 * Prints the partition as bitstrings of the non-empty components to the terminal.
 * 
 * @param partition             Partition as a vector of n integers representing the components.
 * 
 * @return void                 Nothing is returned by this function.
 */
void print_partition_to_terminal(std::vector<__uint128_t>& partition);