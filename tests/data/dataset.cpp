#include "gtest/gtest.h"
#include "../../include/data/dataset.h"

TEST(dataset, init){
    // Declare variables
    int q = 3;
    int n = 3;

    // Wrong file name
    try {
        Data data("non_existing_file.dat", n, q);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("Not able to open the file."));
    }

    // Too large of a system
    n = 129;
    try {
        Data data("../tests/test.dat", n, q);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("The maximum system size is 128 variables."));
    }
    
    // Too small of a system
    n = 0;
    try {
        Data data("../tests/test.dat", n, q);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("The system size should be at least 1."));
    }

    // Too many variables
    n = 4;
    try {
        Data data("../tests/test.dat", n, q);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("File contains datapoints with less than n variables."));
    }

    // Too low value of q
    n = 3;
    q = 2;
    try {
        Data data("../tests/test.dat", n, q);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("The dataset should only contain values between 0 and q-1."));
    }

    // Normal init
    q = 3;
    Data data("../tests/test.dat", n, q);

    std::vector<__uint128_t> powers = {1,3,9, 27};
    EXPECT_EQ(powers, data.pow_q);
    EXPECT_EQ(data.n, n);
    EXPECT_EQ(data.q, q);
    EXPECT_EQ(data.N, 7);
    EXPECT_EQ(data.n_ints, 2);
    EXPECT_EQ(data.N_unique, 6);
    EXPECT_EQ(data.dataset.size(), 6);
}

TEST(dataset, entropy){
    // Declare variables
    int q = 3;
    int n = 3;

    Data data("../tests/test.dat", n, q);

    // Invalid base
    try {
        data.entropy(1);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("Base should be a positive integer larger than or equal to 2."));
    }

    // Base 2
    EXPECT_FLOAT_EQ(data.entropy(2), 2.521640636343318);
    // Base 3
    EXPECT_FLOAT_EQ(data.entropy(), 1.5909781052838627);
}