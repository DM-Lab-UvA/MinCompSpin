#pragma once

#include <iostream>

/**
 * Calculates the bit count of an integer
 * 
 * @param integer             A 128bit integer
 *
 *@return The number of bits set to one. 
 */
int bit_count(__uint128_t integer);

int randomBitIndex(__uint128_t integer);

__uint128_t random_128_int(int n);

/**
 * Converts an integer to the corresponding binary string representation.
 * 
 * @param integer               Integer representation of the bitstring.
 * @param n                     Number of variables in the system.
 * 
 * @return The binary string representation
 */
std::string int_to_string(__uint128_t integer, int n);

__uint128_t string_to_int(std::string string, int n);