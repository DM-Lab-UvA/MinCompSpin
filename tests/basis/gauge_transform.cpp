#include "gtest/gtest.h"
#include "basis/basis.h"

TEST(gauge_transform, data_in_place){
    // Create data object
    int n = 2;
    int q = 3;
    Data data("../tests/test_gt.dat", n, q);
    
    // Ordering of the states in the data object
    // 00  20  02  22  10  12  01  21  11

    // Create basis object
    std::vector<std::vector<uint8_t>> spin_ops = {{1,1}, {2,1}}; // s1 s2, s1^2 s2
    Basis basis(n, q, spin_ops);

    // Transform data in place
    basis.gt_data_in_place(data);
    std::vector<std::vector<__uint128_t>> gt_states = {{0,0}, {2,1}, {0,3},  // 00  21  22
                                                       {1,0}, {1,2}, {2,0},  // 10  12  01 
                                                       {3,0}, {0,2}, {0,1}}; // 11  02  20

    std::vector<int> counts = {3,1,2,1,1,1,1,2,1};
    EXPECT_EQ(data.N, 13);
    EXPECT_EQ(data.N_unique, 9);
    EXPECT_EQ(data.dataset.size(), 9);
    for (int i = 0; i < 9; ++i){
        EXPECT_EQ(data.dataset[i].first, gt_states[i]);
        EXPECT_EQ(data.dataset[i].second, counts[i]);
    }
}

TEST(gauge_transform, data_copy){
    // Create data object
    int n = 2;
    int q = 3;
    Data data("../tests/test_gt.dat", n, q);
    
    std::vector<std::vector<__uint128_t>> states = {{0,0}, {0,1}, {0,2},  // 00  20  02
                                                    {0,3}, {1,0}, {1,2},  // 22  10  12 
                                                    {2,0}, {2,1}, {3,0}}; // 01  21  11

    // Create basis object
    std::vector<std::vector<uint8_t>> spin_ops = {{1,1}, {2,1}}; // s1 s2, s1^2 s2
    Basis basis(n, q, spin_ops);

    // Transform data in place
    Data data_copy = basis.gt_data(data);
    std::vector<std::vector<__uint128_t>> gt_states = {{0,0}, {2,1}, {0,3},  // 00  21  22
                                                        {1,0}, {1,2}, {2,0},  // 10  12  01 
                                                        {3,0}, {0,2}, {0,1}}; // 11  02  20

    std::vector<int> counts = {3,1,2,1,1,1,1,2,1};
    EXPECT_EQ(data_copy.N, 13);
    EXPECT_EQ(data_copy.N_unique, 9);
    EXPECT_EQ(data_copy.dataset.size(), 9);
    for (int i = 0; i < 9; ++i){
        EXPECT_EQ(data.dataset[i].first, states[i]);
        EXPECT_EQ(data_copy.dataset[i].first, gt_states[i]);
        EXPECT_EQ(data_copy.dataset[i].second, counts[i]);
    }
}

TEST(gauge_transform, basis_in_place){
    // Set variables
    int n = 3;
    int q = 2;

    // Basis 1: s1s2, s1, s3
    std::vector<std::vector<uint8_t>> spin_ops_1 = {{1,1,0}, {1,0,0}, {0,0,1}}; 
    Basis basis1(n, q, spin_ops_1);

    // Basis 2: s1s3, s2, s1s2
    std::vector<std::vector<uint8_t>> spin_ops_2 = {{1,0,1}, {0,1,0}, {1,1,0}}; 
    Basis basis2(n, q, spin_ops_2);

    basis2.gt_basis_in_place(basis1);

    // Expected transform of basis 1: s1s2s3, s1, s2
    std::vector<std::vector<uint8_t>> exp_spin_ops_matrix = {{1,1,0}, {1,0,1}, {1,0,0}};
    std::vector<std::vector<__uint128_t>> exp_spin_ops = {{7}, {1}, {2}};

    EXPECT_EQ(basis1.get_basis(), exp_spin_ops);
    EXPECT_EQ(basis1.get_basis_matrix(), exp_spin_ops_matrix);
}