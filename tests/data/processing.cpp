#include "gtest/gtest.h"
#include "../../include/data/data_processing.h"

TEST(data, read_in){
    // Declare variables
    std::vector<std::pair<std::vector<__uint128_t>, unsigned int>> data;
    int q;
    int n;
    int n_ints;
    int N;

    // Wrong file name
    try {
        processing("non_existing_file.dat", 9, 2, 5, data);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("Not able to open the file."));
    }

    // Too many variables
    n = 4;
    try {
        processing("../tests/test.dat", n, 2, 5, data);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("File contains datapoints with less than n variables."));
    }

    // Too low value of q
    n = 3;
    q = 2;
    try {
        processing("../tests/test.dat", n, 2, q, data);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("Entries in the file should only contain values between 0 and q-1."));
    }

    // Read in normally
    n = 3;
    q = 3;
    n_ints = 2; 
    N = processing("../tests/test.dat", n, n_ints, q, data);

    EXPECT_EQ(N, 7);
    EXPECT_EQ(data.size(), 6);

    // Read in with fewer variables
    n = 2;
    data.clear();
    N = processing("../tests/test.dat", n, n_ints, q, data);
    EXPECT_EQ(N, 7);
    EXPECT_EQ(data.size(), 5);  

    // Read in with additional integers
    n_ints = 5;
    data.clear();
    N = processing("../tests/test.dat", n, n_ints, q, data);
    EXPECT_EQ(data[0].first.size(), n_ints);
}