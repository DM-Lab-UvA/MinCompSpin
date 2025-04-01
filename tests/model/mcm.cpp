#include "gtest/gtest.h"
#include "../../include/model/mcm.h"

TEST(model, init_n) {
    // Declare variable
    int n;

    // Invalid number of variables
    n = 0;
    try {
        MCM mcm(n);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("The number of variables should be a non-zero positive number."));
    }

    // Too many variables
    n = 129;
    try {
        MCM mcm(n);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("The maximum system size is 128 variables."));
    }

    // Valid number of variables
    n = 2;
    MCM mcm(n);
    std::vector<__uint128_t> partition = {1,2};

    EXPECT_EQ(mcm.n, n);
    EXPECT_EQ(mcm.n_comp, n);
    EXPECT_EQ(mcm.rank, n);
    EXPECT_EQ(mcm.partition, partition);
    EXPECT_FALSE(mcm.optimized);
}

TEST(model, init_partition) {
    // Declare variable
    std::vector<__uint128_t> partition;
    int n;

    // Invalid number of variables
    n = 0;
    try {
        MCM mcm(n, partition);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("The number of variables should be a non-zero positive number."));
    }

    // Too many variables
    n = 129;
    try {
        MCM mcm(n);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("The maximum system size is 128 variables."));
    }

    // Valid partition
    partition = {4,1,2};
    n = 3;
    MCM mcm(n, partition);
    
    EXPECT_EQ(mcm.n, n);
    EXPECT_EQ(mcm.n_comp, 3);
    EXPECT_EQ(mcm.rank, n);
    EXPECT_EQ(partition, mcm.partition);
    EXPECT_FALSE(mcm.optimized);

    // Valid partition with additional empty components
    n = 4;
    partition = {0, 0, 0, 12, 0, 0, 0, 0, 3};
    mcm = MCM(n, partition);
    EXPECT_EQ(mcm.n, n);
    EXPECT_EQ(mcm.n_comp, 2);
    EXPECT_EQ(mcm.rank, n);

    std::vector<__uint128_t> exp_partition = {12, 3, 0, 0};
    EXPECT_EQ(mcm.partition.size(), n);
    EXPECT_EQ(exp_partition, mcm.partition);

    // Too many variables (n+1 th variable is present)
    n = 3;
    partition = {8, 0, 0};
    try {
        mcm = MCM(n, partition);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("Invalid partition because there are more than n variables."));
    }

    // Same variable in two different components
    partition = {1, 1, 2};
    try {
        mcm = MCM(n, partition);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("Invalid partition because the same variable occurs in multiple components."));
    }
    
    // Partition with fewer variables
    partition = {1,4};
    mcm = MCM(n, partition);
    exp_partition = {1,4,0};

    EXPECT_EQ(mcm.n, n);
    EXPECT_EQ(mcm.n_comp, 2);
    EXPECT_EQ(mcm.rank, 2);
    EXPECT_EQ(mcm.partition, exp_partition);
}

TEST(model, init_string){
    // Declare variable
    std::string partition;
    int n;

    // Invalid number of variables
    n = 0;
    try {
        MCM mcm(n, partition);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("The number of variables should be a non-zero positive number."));
    }

    // Too many variables
    n = 129;
    try {
        MCM mcm(n);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("The maximum system size is 128 variables."));
    }

    // Invalid string
    n = 2;
    partition = "invalid_partition_type";
    try {
        MCM mcm(n, partition);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("Invalid partition type. Options are 'independent', 'complete' or 'random'."));
    }

    // Independent model
    n = 5;
    partition = "independent";
    MCM mcm(n, partition);

    std::vector<__uint128_t> exp_partition = {1,2,4,8,16};
    EXPECT_EQ(mcm.n, n);
    EXPECT_EQ(mcm.n_comp, n);
    EXPECT_EQ(mcm.rank, n);
    EXPECT_EQ(mcm.partition.size(),  n);
    EXPECT_EQ(mcm.partition, exp_partition);
    EXPECT_FALSE(mcm.optimized);

    // Complete model
    partition = "complete";
    mcm = MCM(n, partition);

    exp_partition = {31, 0, 0, 0, 0};
    EXPECT_EQ(mcm.n_comp, 1);
    EXPECT_EQ(mcm.rank, n);
    EXPECT_EQ(mcm.partition.size(),  n);
    EXPECT_EQ(mcm.partition, exp_partition);
    EXPECT_FALSE(mcm.optimized);
}

TEST(model, get_partition){
    // Create model without partition
    int n = 4;
    MCM mcm(n);
    std::vector<__uint128_t> partition = mcm.get_partition();
    std::vector<__uint128_t> exp_partition = {1,2,4,8};

    EXPECT_EQ(partition, exp_partition);

    // After init with partition
    mcm = MCM(n, "complete");
    exp_partition = {15,0,0,0};
    partition = mcm.get_partition();

    EXPECT_EQ(partition, exp_partition);
}

TEST(model, set_partition){
    // Declare variables
    int n;
    std::vector<__uint128_t> partition;

    // Partition with too many components
    n = 3;
    partition = {1,2,4,0};
    MCM mcm(n);

    try {
        mcm.set_starting_partition(partition);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("The given partition contains more than n components."));
    }

    // Same variable twice
    partition = {1,1,2};

    try {
        mcm.set_starting_partition(partition);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("Invalid partition because the same variable occurs in multiple components."));
    } 

    // Too many variables
    partition = {8,0,0};

    try {
        mcm.set_starting_partition(partition);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("Invalid partition because there are more than n variables."));
    }

    // Setting valid partition
    partition = {4,3,0};
    mcm.set_starting_partition(partition);

    EXPECT_EQ(mcm.n, n);
    EXPECT_EQ(mcm.rank, n);
    EXPECT_EQ(mcm.n_comp, 2);
    EXPECT_EQ(mcm.partition.size(), n);
    EXPECT_EQ(mcm.partition, partition);

    // Setting valid partition with fewer non-empty component
    partition = {7,0,0};
    mcm.set_starting_partition(partition);

    EXPECT_EQ(mcm.n, n);
    EXPECT_EQ(mcm.rank, n);
    EXPECT_EQ(mcm.n_comp, 1);
    EXPECT_EQ(mcm.partition.size(), n);
    EXPECT_EQ(mcm.partition, partition);

    // Resetting previous search result after setting starting partition
    std::vector<double> log_ev_per_icc = {-2, 0, 0};
    mcm.log_ev_per_icc = log_ev_per_icc;
    mcm.optimized = true;
    EXPECT_EQ(mcm.log_ev_per_icc.size(), 3);

    partition = {1,2,4};
    mcm.set_starting_partition(partition);

    EXPECT_EQ(mcm.n_comp, 3);
    EXPECT_EQ(mcm.rank, 3);
    EXPECT_EQ(mcm.partition.size(), n);
    EXPECT_EQ(mcm.partition, partition);
    EXPECT_EQ(mcm.log_ev_per_icc.size(), 0);
    EXPECT_FALSE(mcm.optimized);

    // Empty components first
    partition = {0,1,4};
    std::vector<__uint128_t> exp_partition = {1,4,0};
    mcm.set_starting_partition(partition);

    EXPECT_EQ(mcm.n_comp, 2);
    EXPECT_EQ(mcm.rank, 2);
    EXPECT_EQ(mcm.partition.size(), n);
    EXPECT_EQ(mcm.partition, exp_partition);
    EXPECT_EQ(mcm.log_ev_per_icc.size(), 0);
    EXPECT_FALSE(mcm.optimized);
}

TEST(model, get_best_ev){
    // Create model
    int n = 6;
    std::string partition = "independent";
    MCM mcm(n, partition);

    // No search has been ran -> best_ev should be inaccesible
    try {
        mcm.get_best_log_ev();
        FAIL() << "Expected std::runtime_error";
    }
    catch(std::runtime_error const & err) {
        EXPECT_EQ(err.what(), std::string("No search has been ran yet."));
    }
    try {
        mcm.get_best_log_ev_per_icc();
        FAIL() << "Expected std::runtime_error";
    }
    catch(std::runtime_error const & err) {
        EXPECT_EQ(err.what(), std::string("No search has been ran yet."));
    } 
}
