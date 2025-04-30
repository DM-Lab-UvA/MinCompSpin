#pragma once

#include <cstdint>
#include <vector>
#include <algorithm>

#include "data/dataset.h"
#include "basis/independence.h"

/**
 * Count the number of bits set to 1 in an integer.
 * 
 * @param value                     A 128bit integer.
 * 
 * @return The number of bits set to in the given integer
 */
int count_set_bits(__uint128_t value);

/**
 * Calculate the spin value for a given state and operator.
 * 
 * @param state                     Datapoint represented as a vector of log2(q) 128bit integers.
 * @param op                        Spin operator represented as a vector of log2(q) 128bit integers.
 * @param q                         Number of values each variable can take.
 */
int spin_value(const std::vector<__uint128_t>& state, const std::vector<__uint128_t>& op, int q);

/**
 * Calculate the entropy of an operator.
 * 
 * @param data                      Data object for which the entropy of the spin operator will be calculated.
 * @param op                        Spin operator represented as a vector of 128bit integers.
 */
double calc_entropy_of_spin_op(const Data& data, const std::vector<__uint128_t>& op);

/**
 * Compare the entropy of two spin operators.
 * Used in the sorting algorithm of the basis search.
 * 
 * @param op1                       Pair containing operator 1 as vector of n uint8 and its entropy.
 * @param op2                       Pair containing operator 2 as vector of n uint8 and its entropy.
 * 
 * @return True if entropy of operator 1 is lower than the entropy of operator 2, False otherwise.
 */
bool comp_entropy(const std::pair<std::vector<uint8_t>, double>& op1, const std::pair<std::vector<uint8_t>, double>& op2);

/**
 * Check if the spin operator is valid for a GT.
 * 
 * @param op                        Spin operator as vector of n uint8.
 * @param q                         Number of values each variable can take.
 */
bool valid_operator(const std::vector<uint8_t>& op, int q);

/**
 * Generate all unique colors of spin operators that are valid for a GT.
 * 
 * @param colors                    Empty 2D vector that will contain the colors of spin operators.
 * @param order                     The interaction order of spin operators that will be considered.
 * @param q                         Number of values each variable can take.
 */
void generate_colors_of_spin_ops(std::vector<std::vector<uint8_t>>& colors, int order, int q);

/**
 * Calculate and store the entropy of spin operators of a given order.
 * 
 * @param data                      Data object for which the entropy of operators will be calculated.
 * @param entropy_of_ops            Vector of pairs containing the spin operators and the corresponding entropy.
 * @param order                     The interaction order of spin operators that will be considered.
 */
void entropy_of_spin_ops_of_order_k(const Data& data, std::vector<std::pair<std::vector<uint8_t>, double>>& entropy_of_ops, int order);

/**
 * Sort the spin operators up to a given order from low to high entropy.
 * 
 * @param data                      Data object for which the order of the spin operators is determined.
 * @param spin_ops                  Empty 2D matrix that will contain the ordered spin operators as columns.
 * @param max_order                 The maximum interaction order of the operators to consider in the process.
 */
void sort_spin_ops(const Data& data, std::vector<std::vector<uint8_t>>& spin_ops, int max_order);
