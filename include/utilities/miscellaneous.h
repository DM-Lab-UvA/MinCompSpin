#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <cstdint>

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

/**
 * Converts from a string to log2(q) 128bit integers.
 * 
 * @param vec                   Vector of log2(q) 128bit integers that will contain the converted representation.
 * @param str                   String of length n with values between 0 and q-1.
 * @param n                     Number of variables in the system.
 * @param q                     Number of states.
 * 
 * @return void                 Nothing is returned by this function.
 */
void convert_string_to_vector(std::vector<__uint128_t>& vec, std::string& str, int n, int q);

/**
 * Converts from a n 8bit integer representation to log2(q) 128bit integer representation.
 * 
 * @param vec                   Vector of log2(q) 128bit integers that will contain the converted representation.
 * @param str                   String of length n with values between 0 and q-1.
 * @param n                     Number of variables in the system.
 * @param q                     Number of states.
 * 
 * @return void                 Nothing is returned by this function.
 */
void convert_8bit_vec_to_128bit_vec(std::vector<__uint128_t>& vec128, std::vector<uint8_t>& vec8, int n, int q);