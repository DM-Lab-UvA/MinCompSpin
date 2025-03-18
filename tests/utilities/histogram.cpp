#include "gtest/gtest.h"
#include "../../include/data/data_processing.h"
#include "../../include/utilities/histogram.h"

TEST(histogram, component){
    // Declare variables
    int q = 3;
    int n = 3;
    std::unordered_map<std::vector<__uint128_t>, unsigned int, HashVector128> freqs;

    Data data("../tests/test.dat", n, q);
    freqs = build_histogram(data, 1);

    // Community = 1
    std::vector<__uint128_t> key = {0,0};
    EXPECT_EQ(freqs.size(), 3);
    EXPECT_EQ(freqs[key], 1);
    key = {1,0};
    EXPECT_EQ(freqs[key], 2);
    key = {0,1};
    EXPECT_EQ(freqs[key], 4);

    // Community = 2
    freqs = build_histogram(data, 2);
    EXPECT_EQ(freqs.size(), 3);
    key = {0,0};
    EXPECT_EQ(freqs[key], 1);
    key = {2,0};
    EXPECT_EQ(freqs[key], 5);
    key = {0,2};
    EXPECT_EQ(freqs[key], 1);

    // Community = 4
    freqs = build_histogram(data, 4);
    EXPECT_EQ(freqs.size(), 3);
    key = {0,0};
    EXPECT_EQ(freqs[key], 4);
    key = {4,0};
    EXPECT_EQ(freqs[key], 2);
    key = {0,4};
    EXPECT_EQ(freqs[key], 1);

    // Community = 3
    freqs = build_histogram(data, 3);
    EXPECT_EQ(freqs.size(), 5);
    key = {0,1};
    EXPECT_EQ(freqs[key], 1);
    key = {1,2};
    EXPECT_EQ(freqs[key], 1);
    key = {3,0};
    EXPECT_EQ(freqs[key], 1);
    key = {2,1};
    EXPECT_EQ(freqs[key], 3);
    key = {2,0};
    EXPECT_EQ(freqs[key], 1);

    // Community = 7
    freqs = build_histogram(data, 7);
    EXPECT_EQ(freqs.size(), 6);
    key = {0,1};
    EXPECT_EQ(freqs[key], 1);
    key = {2,1};
    EXPECT_EQ(freqs[key], 2);
    key = {1,2};
    EXPECT_EQ(freqs[key], 1);
    key = {6,1};
    EXPECT_EQ(freqs[key], 1);
    key = {7,0};
    EXPECT_EQ(freqs[key], 1);
    key = {2,4};
    EXPECT_EQ(freqs[key], 1);
}

TEST(histogram, complete){
    // Declare variables
    int q = 3;
    int n = 3;
    std::unordered_map<std::vector<__uint128_t>, unsigned int, HashVector128> freqs;

    Data data("../tests/test.dat", n, q);

    freqs = build_histogram(data);
    
    EXPECT_EQ(freqs.size(), 6);
    std::vector<__uint128_t> key = {0,1};
    EXPECT_EQ(freqs[key], 1);
    key = {2,1};
    EXPECT_EQ(freqs[key], 2);
    key = {1,2};
    EXPECT_EQ(freqs[key], 1);
    key = {6,1};
    EXPECT_EQ(freqs[key], 1);
    key = {7,0};
    EXPECT_EQ(freqs[key], 1);
    key = {2,4};
    EXPECT_EQ(freqs[key], 1);
}