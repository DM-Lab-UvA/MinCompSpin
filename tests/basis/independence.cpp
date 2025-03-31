#include "gtest/gtest.h"
#include "basis/independence.h"

TEST(independence, row_reduce){
    std::vector<uint8_t> cols;
    std::vector<uint8_t> exp_cols;
    int q = 4;

    // Valid basis
    exp_cols = {0,1};

    // Basis: s1, s2
    std::vector<std::vector<uint8_t>> matrix = {{1,0}, {0,1}};
    cols = row_reduce(matrix, q);    
    EXPECT_EQ(cols, exp_cols);

    // Basis: s1, s1^2 s2
    matrix = {{1,2}, {0,1}};
    cols = row_reduce(matrix, q);
    EXPECT_EQ(cols, exp_cols);

    // Basis: s1 s2, s1^2 s2
    matrix = {{1,2}, {1,1}};
    cols = row_reduce(matrix, q);
    EXPECT_EQ(cols, exp_cols);

    // Invalid basis
    exp_cols = {0};

    // Basis: s1 s2, s1 s2
    matrix = {{1,1}, {1,1}};
    cols = row_reduce(matrix, q);
    EXPECT_EQ(cols, exp_cols);

    // Basis: s1, s1 s2^2
    matrix = {{1,1}, {0,2}};
    cols = row_reduce(matrix, q);
    EXPECT_EQ(cols, exp_cols);

    // Basis: s1^2 s2^2, s2
    exp_cols = {1};
    matrix = {{2,0}, {2,1}};
    cols = row_reduce(matrix, q);
    EXPECT_EQ(cols, exp_cols);

    // Basis: s1^2, s2^2
    matrix = {{2,0}, {0,2}};
    cols = row_reduce(matrix, q);
    EXPECT_EQ(cols.size(), 0);

}

TEST(independence, check){
    int q = 4;
    int n = 2;

    std::vector<std::vector<uint8_t>> matrix = {{1,0}, {0,1}};
    EXPECT_TRUE(is_linearly_independent(matrix, n, q));

    matrix = {{1,2}, {0,1}};
    EXPECT_TRUE(is_linearly_independent(matrix, n, q));

    matrix = {{1,2}, {1,1}};
    EXPECT_TRUE(is_linearly_independent(matrix, n, q));

    n = 3;
    EXPECT_FALSE(is_linearly_independent(matrix, n, q));

    matrix = {{1,1}, {1,1}};
    EXPECT_FALSE(is_linearly_independent(matrix, n, q));

    n = 2;
    EXPECT_FALSE(is_linearly_independent(matrix, n, q));

    matrix = {{1,1}, {0,2}};
    EXPECT_FALSE(is_linearly_independent(matrix, n, q));

    matrix = {{2,0}, {2,1}};
    EXPECT_FALSE(is_linearly_independent(matrix, n, q));    

    matrix = {{2,0}, {0,2}};
    EXPECT_FALSE(is_linearly_independent(matrix, n, q));

    matrix = {{2,1}, {1,0}};
    EXPECT_TRUE(is_linearly_independent(matrix, n, q));

    q = 9;
    matrix = {{3,1}, {2,0}};
    EXPECT_TRUE(is_linearly_independent(matrix, n, q));

    matrix = {{3,0}, {2,1}};
    EXPECT_FALSE(is_linearly_independent(matrix, n, q));
}