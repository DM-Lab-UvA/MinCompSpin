#include "gtest/gtest.h"
#include "../../include/data/dataset.h"

TEST(evidence, icc){
    // Declare variables
    int q = 3;
    int n = 3;

    Data data("../tests/test.dat", n, q);

    EXPECT_FLOAT_EQ(data.calc_log_ev_icc(1),  -8.769507120030227);
    EXPECT_FLOAT_EQ(data.calc_log_ev_icc(2),  -7.670894831362117);
    EXPECT_FLOAT_EQ(data.calc_log_ev_icc(4),  -8.769507120030227);
    EXPECT_FLOAT_EQ(data.calc_log_ev_icc(3),  -15.982243968542207);
    EXPECT_FLOAT_EQ(data.calc_log_ev_icc(7),  -23.324842793537606);
}

TEST(evidence, total){
    // Declare variables
    int q = 3;
    int n = 3;

    Data data("../tests/test.dat", n, q);

    std::vector<__uint128_t> partition = {1,2,4};
    EXPECT_EQ(data.calc_log_ev(partition), data.calc_log_ev_icc(1) + data.calc_log_ev_icc(2) + data.calc_log_ev_icc(4));

    partition = {7,0,0};
    EXPECT_EQ(data.calc_log_ev(partition), data.calc_log_ev_icc(7));

    partition = {3,4,0};
    EXPECT_EQ(data.calc_log_ev(partition), data.calc_log_ev_icc(3) + data.calc_log_ev_icc(4));
    
    // Subcomplete models
    partition = {4,0,0};
    EXPECT_EQ(data.calc_log_ev(partition), data.calc_log_ev_icc(4) - 7 * 2 * log(3));

    partition = {1,4,0};
    EXPECT_EQ(data.calc_log_ev(partition), data.calc_log_ev_icc(4) + data.calc_log_ev_icc(1) - 7 * log(3));
}
