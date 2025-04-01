# pragma once

#include <cstdint>
#include <vector>

int gcd(int a, int b);

bool is_linearly_independent(std::vector<std::vector<uint8_t>>& matrix, int n, int q);

std::vector<uint8_t> row_reduce(std::vector<std::vector<uint8_t>> matrix, int q);