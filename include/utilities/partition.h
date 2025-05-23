#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cmath>

#include "model/mcm.h"

// Forward declaration
class MCM;

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
 * Print the partition as bitstrings of the non-empty components to a file.
 * 
 * @param file                  Path to the file where the partition should be written.
 * @param partition             Partition as a vector of n integers representing the components.
 * 
 * @return void                 Nothing is returned by this function.
 */
void print_partition_to_file(std::ofstream& file, std::vector<__uint128_t>& partition);

/**
 * Print the partition as bitstrings of the non-empty components to a file with evidence per icc.
 * 
 * @param file                  Path to the file where the partition should be written.
 * @param mcm                   MCM object containing information about the partition and evidence.
 * @param N                     Number of datapoints in the dataset.
 * @param q                     Number of states each variable can take.
 */
void print_partition_details_to_file(std::ofstream& file, MCM& mcm, int N, int q);

/**
 * Print the partition as bitstrings of the non-empty components to the terminal.
 * 
 * @param partition             Partition as a vector of n integers representing the components.
 * 
 * @return void                 Nothing is returned by this function.
 */
void print_partition_to_terminal(std::vector<__uint128_t>& partition);

/**
 * Place the empty entries in a vector at the end.
 * 
 * @param vector                Vector containing elements of type T.
 */
template <typename T>
void place_empty_entries_last(std::vector<T>& vector){
    int n = vector.size() - 1;
    for (int i = 0; i < vector.size(); i++){
        if (! vector[i]){
            // Empty component --> switch with the last filled component
            while (! vector[n]){n--;}
            // Quit when last filled component is located before the first empty component
            if (n < i){break;}
            // Switch
            vector[i] = vector[n];
            vector[n] = 0;
            n--;
        }
    }
}