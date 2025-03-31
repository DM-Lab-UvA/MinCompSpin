#include "gtest/gtest.h"
#include "basis/basis.h"

TEST(gauge_transform, count_bits){
    EXPECT_EQ(count_set_bits(0), 0);
    EXPECT_EQ(count_set_bits(1), 1);
    EXPECT_EQ(count_set_bits(2), 1);
    EXPECT_EQ(count_set_bits(3), 2);
    EXPECT_EQ(count_set_bits(26), 3);
}

TEST(gauge_transform, spin_val){
    std::vector<__uint128_t> state;
    std::vector<__uint128_t> op;

    // Base 2
    int q = 2;

    state = {0}; // 00
    op = {0}; // 00
    EXPECT_EQ(spin_value(state, op, q), 0);

    state = {2}; // 01
    EXPECT_EQ(spin_value(state, op, q), 0);

    op = {1}; // 10
    EXPECT_EQ(spin_value(state, op, q), 0);

    op = {2}; // 01
    EXPECT_EQ(spin_value(state, op, q), 1);

    op = {3}; // 11
    EXPECT_EQ(spin_value(state, op, q), 1);

    state = {3}; // 11
    EXPECT_EQ(spin_value(state, op, q), 0);

    op = {2}; // 10
    EXPECT_EQ(spin_value(state, op, q), 1);

    // Base 3
    q = 3;

    state = {1,0}; // 10
    op = {1,0}; // 10
    EXPECT_EQ(spin_value(state, op, q), 1);

    state = {0,1}; // 20
    EXPECT_EQ(spin_value(state, op, q), 2);

    op = {3,0}; // 11
    EXPECT_EQ(spin_value(state, op, q), 2);

    state = {2,1}; // 21
    EXPECT_EQ(spin_value(state, op, q), 0);

    op = {1,2}; // 12
    EXPECT_EQ(spin_value(state, op, q), 1);
    
    // Base 8
    q = 8;

    state = {3,3,2}; // 37
    op = {0,3,1}; // 62
    EXPECT_EQ(spin_value(state, op, q), 0); 
}

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