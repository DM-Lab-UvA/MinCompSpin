#pragma once

#include "mcm_search.h"
#include "utilities/partition.h"

/**
 * Helper function for the exhaustive search that updates the partition.
 * 
 * @param a                     Array of size n that represents the partition as a restricted growth string.
 * @param b                     Array of size n that keeps track of how many partitions each variable can move to.
 * @param n                     Number of variables.
 * 
 * @return 1 if next partition is generated, 0 if all partitions are generated.
 */

int generate_next_partition(int* a, int* b, int n);
/**
 * Helper function for generating partition that updates the partition.
 * 
 * @param a                     Array of size n.
 * @param b                     Array of size n.
 * @param n                     Size of the arrays.
 * 
 * @return Index of the first bit from right to left that is different between a and b.
 */
int find_j(int* a, int* b, int n);
