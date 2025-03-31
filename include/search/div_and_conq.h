#pragma once

#include "search/search.h"

/**
 * Helper function for the division procedure that finds the integer representation of the bitstring with only a 1 in the position of the ith bit set to 1 in component.
 * 
 * @param component             Integer representation of the bitstring representing a component.
 * @param i                     Index of the bit to be found.
 * 
 * @return Integer representation of the bitstring with only a 1 in the position of the ith bit set to 1 in component.
 */
__uint128_t find_member_i(__uint128_t component, int i);

/**
 * Helper function when printing the steps of the algorithm to a file that locates the position of the (first) bit set to 1.
 * 
 * @param member                Integer representation of a bitstring
 * 
 * @return Index of the least significant bit set to 1.
 */
int index_of_member(__uint128_t member);
