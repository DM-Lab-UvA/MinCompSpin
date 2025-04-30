#pragma once

#include <chrono>

#include "data/dataset.h"
#include "basis/basis.h"

class BasisSearch {
public:
    /**
     * Construct a BasisSearch object containing methods to find the best basis representation for a dataset.
     */
    BasisSearch();

    // Search methods
    Basis up_to_order_k(const Data& data, int k, const std::string file_name = "");
    Basis iterative_search(const Data& data, int k, const std::string file_name = "");
    Basis exhaustive_search(const Data& data, const std::string file_name = "") {return up_to_order_k(data, data.n, file_name);};

    Basis get_basis();
    void print_search_results();

private:
    Basis basis;
    std::string search_method;
    int n_iterations;
    int max_order;
    double duration;
};

bool is_identity(const std::vector<std::vector<uint8_t>>& matrix, int n);