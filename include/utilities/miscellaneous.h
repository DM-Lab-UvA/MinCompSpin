#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <cstdint>

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
void convert_8bit_vec_to_128bit_vec(std::vector<__uint128_t>& vec128, const std::vector<uint8_t>& vec8, int n, int q);

/**
 * Unsafe conversion from a n 8bit integer representation to log2(q) 128bit integer representation.
 * Doesn't check whether the given operator is valid. 
 * Used internally to speed up basis search algorithm if operator is guaranteed to be valid.
 * 
 * @param vec                   Vector of log2(q) 128bit integers that will contain the converted representation.
 * @param str                   String of length n with values between 0 and q-1.
 * @param n                     Number of variables in the system.
 * @param q                     Number of states.
 * 
 * @return void                 Nothing is returned by this function.
 */
void convert_8bit_vec_to_128bit_vec_unsafe(std::vector<__uint128_t>& vec128, const std::vector<uint8_t>& vec8, int n, int q);

/**
 * Convert from log2(q) 128bit integer representation to a string of length n with entries between 0 and q-1.
 * 
 * @param vec128                Vector of log2(q) 128bit integers.
 * @param n                     Number of variables in the system.
 */
std::string convert_128bit_vec_to_string(std::vector<__uint128_t>& vec128, int n);