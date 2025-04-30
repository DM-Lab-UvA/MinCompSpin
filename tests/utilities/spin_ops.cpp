#include "gtest/gtest.h"
#include "utilities/spin_ops.h"

TEST(spin_ops, count_bits){
    EXPECT_EQ(count_set_bits(0), 0);
    EXPECT_EQ(count_set_bits(1), 1);
    EXPECT_EQ(count_set_bits(2), 1);
    EXPECT_EQ(count_set_bits(3), 2);
    EXPECT_EQ(count_set_bits(26), 3);
}

TEST(spin_ops, spin_val){
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

TEST(spin_op, entropy){
    // Variables
    int n = 3;
    int q = 3;
    std::vector<__uint128_t> op;
    // Create data object
    Data data("../tests/test.dat", n, q);

    // s1
    op = {1,0};
    EXPECT_FLOAT_EQ(calc_entropy_of_spin_op(data, op), 0.9556998911125343);

    // s2
    op = {2,0};
    EXPECT_FLOAT_EQ(calc_entropy_of_spin_op(data, op), 0.7963116401738131);

    // s3
    op = {4,0};
    EXPECT_FLOAT_EQ(calc_entropy_of_spin_op(data, op), 0.9556998911125343);

    // s1 s2
    op = {3,0};
    EXPECT_FLOAT_EQ(calc_entropy_of_spin_op(data, op), 0.9556998911125343);

    // s1 s3
    op = {5,0};
    EXPECT_FLOAT_EQ(calc_entropy_of_spin_op(data, op), 0.7963116401738131);

    // s2 s3
    op = {6,0};
    EXPECT_FLOAT_EQ(calc_entropy_of_spin_op(data, op), 1.0789922078775833);

    // s1 s2 s3
    op = {7,0};
    EXPECT_FLOAT_EQ(calc_entropy_of_spin_op(data, op), 0.7963116401738131);

    // s1 s2^2
    op = {1,2};
    EXPECT_FLOAT_EQ(calc_entropy_of_spin_op(data, op), 1.0042424730540764);

    // s1 s3^2
    op = {1,4};
    EXPECT_FLOAT_EQ(calc_entropy_of_spin_op(data, op), 1.0042424730540764);

    // s2 s3^2
    op = {2,4};
    EXPECT_FLOAT_EQ(calc_entropy_of_spin_op(data, op), 1.0789922078775833);

    // s1 s2^2 s3^2
    op = {1,6};
    EXPECT_FLOAT_EQ(calc_entropy_of_spin_op(data, op), 1.0789922078775833);

    // s1 s2 s3^2
    op = {3,4};
    EXPECT_FLOAT_EQ(calc_entropy_of_spin_op(data, op), 1.0042424730540764);

    // s1^2 s2^2 s3
    op = {4,3};
    EXPECT_FLOAT_EQ(calc_entropy_of_spin_op(data, op), 1.0042424730540764);

    // s1 s2^2 s3
    op = {5,2};
    EXPECT_FLOAT_EQ(calc_entropy_of_spin_op(data, op), 0.6829081047004717);
}

TEST(spin_ops, valid_op){
    std::vector<uint8_t> op = {2,1,1};
    std::vector<uint8_t> conj_op = {1,2,2};
    EXPECT_TRUE(valid_operator(op, 3));
    EXPECT_FALSE(valid_operator(conj_op, 3));

    op = {1,1};
    EXPECT_TRUE(valid_operator(op, 4));
    op = {3,3};
    EXPECT_FALSE(valid_operator(op, 4));

    op = {1,2};
    EXPECT_TRUE(valid_operator(op, 4));
    op = {3,2};
    EXPECT_FALSE(valid_operator(op, 4));

    op = {1,3};
    EXPECT_FALSE(valid_operator(op, 4));
    op = {3,1};
    EXPECT_TRUE(valid_operator(op, 4));

    op = {2,1};
    EXPECT_TRUE(valid_operator(op, 4));
    op = {2,3};
    EXPECT_FALSE(valid_operator(op, 4));

    op = {2,2};
    EXPECT_FALSE(valid_operator(op, 4));

    op = {0,2};
    EXPECT_FALSE(valid_operator(op, 4));

    op = {2,0};
    EXPECT_FALSE(valid_operator(op, 4));
}

TEST(spin_ops, colors){
    // Variables
    int q = 3;
    int k = 3;
    std::vector<std::vector<uint8_t>> colors;

    generate_colors_of_spin_ops(colors, k, q);
    EXPECT_EQ(colors.size(), 4);

    std::vector<uint8_t> color = {1,1,1};
    EXPECT_EQ(colors[0], color);

    color = {2,1,1};
    EXPECT_EQ(colors[1], color);

    color = {1,2,1};
    EXPECT_EQ(colors[2], color);

    color = {2,2,1};
    EXPECT_EQ(colors[3], color);

    // Variables
    q = 4;
    k = 2;
    colors.clear();

    generate_colors_of_spin_ops(colors, k, q);
    EXPECT_EQ(colors.size(), 4);

    color = {1,1};
    EXPECT_EQ(colors[0], color);

    color = {2,1};
    EXPECT_EQ(colors[1], color);

    color = {3,1};
    EXPECT_EQ(colors[2], color);

    color = {1,2};
    EXPECT_EQ(colors[3], color);

    // Variables
    q = 5;
    k = 1;
    colors.clear();

    generate_colors_of_spin_ops(colors, k, q);
    EXPECT_EQ(colors.size(), 2);

    color = {1};
    EXPECT_EQ(colors[0], color);

    color = {2};
    EXPECT_EQ(colors[1], color);
}

TEST(spin_ops, entropy_of_ops){
    // Variables
    int n = 3;
    int q = 3;
    std::vector<std::pair<std::vector<uint8_t>, double>> entropy_of_ops;

    std::vector<uint8_t> op;
    // Create data object
    Data data("../tests/test.dat", n, q);
    
    // First order
    entropy_of_spin_ops_of_order_k(data, entropy_of_ops, 1);
    EXPECT_EQ(entropy_of_ops.size(), 3);

    op = {1,0,0};
    EXPECT_EQ(entropy_of_ops[0].first, op);
    EXPECT_FLOAT_EQ(entropy_of_ops[0].second, 0.9556998911125343);
    
    op = {0,1,0};
    EXPECT_EQ(entropy_of_ops[1].first, op);
    EXPECT_FLOAT_EQ(entropy_of_ops[1].second, 0.7963116401738131);

    op = {0,0,1};
    EXPECT_EQ(entropy_of_ops[2].first, op);
    EXPECT_FLOAT_EQ(entropy_of_ops[2].second, 0.9556998911125343);

    // Second order
    entropy_of_ops.clear();
    entropy_of_spin_ops_of_order_k(data, entropy_of_ops, 2);
    EXPECT_EQ(entropy_of_ops.size(), 6);

    op = {1,1,0};
    EXPECT_EQ(entropy_of_ops[0].first, op);
    EXPECT_FLOAT_EQ(entropy_of_ops[0].second, 0.9556998911125343);

    op = {2,1,0};
    EXPECT_EQ(entropy_of_ops[1].first, op);
    EXPECT_FLOAT_EQ(entropy_of_ops[1].second, 1.0042424730540764); 

    op = {1,0,1};
    EXPECT_EQ(entropy_of_ops[2].first, op);
    EXPECT_FLOAT_EQ(entropy_of_ops[2].second, 0.7963116401738131);

    op = {2,0,1};
    EXPECT_EQ(entropy_of_ops[3].first, op);
    EXPECT_FLOAT_EQ(entropy_of_ops[3].second, 1.0042424730540764); 

    op = {0,1,1};
    EXPECT_EQ(entropy_of_ops[4].first, op);
    EXPECT_FLOAT_EQ(entropy_of_ops[4].second, 1.0789922078775833);

    op = {0,2,1};
    EXPECT_EQ(entropy_of_ops[5].first, op);
    EXPECT_FLOAT_EQ(entropy_of_ops[5].second, 1.0789922078775833);

    // Third order
    entropy_of_ops.clear();
    entropy_of_spin_ops_of_order_k(data, entropy_of_ops, 3);
    EXPECT_EQ(entropy_of_ops.size(), 4);

    op = {1,1,1};
    EXPECT_EQ(entropy_of_ops[0].first, op);
    EXPECT_FLOAT_EQ(entropy_of_ops[0].second, 0.7963116401738131);

    op = {2,1,1};
    EXPECT_EQ(entropy_of_ops[1].first, op);
    EXPECT_FLOAT_EQ(entropy_of_ops[1].second, 1.0789922078775833);

    op = {1,2,1};
    EXPECT_EQ(entropy_of_ops[2].first, op);
    EXPECT_FLOAT_EQ(entropy_of_ops[2].second, 0.6829081047004717);

    op = {2,2,1};
    EXPECT_EQ(entropy_of_ops[3].first, op);
    EXPECT_FLOAT_EQ(entropy_of_ops[3].second, 1.0042424730540764);
}

TEST(spin_ops, sort_ops){
    // Variables
    int n = 3;
    int q = 3;
    bool op_present;
    std::vector<uint8_t> exp_op;
    std::vector<uint8_t> op;
    // Create data object
    Data data("../tests/test.dat", n, q);

    // Sorting all operators up to second order
    std::vector<std::vector<uint8_t>> spin_ops;
    sort_spin_ops(data, spin_ops, n-1);

    // There should be 9 operators
    EXPECT_EQ(spin_ops.size(), n);
    EXPECT_EQ(spin_ops[0].size(), 9);
    
    // First two operators should be s2, s1 s3

    // Check if s2 is in the first two operators
    op_present = false;
    exp_op = {0,1,0};
    for (int i = 0; i < 2; ++i){
        for (int j = 0; j < 3; ++j){
            op.push_back(spin_ops[j][i]);
        }
        if (op == exp_op){
            op_present = true;
        }
        op.clear();
    }
    EXPECT_TRUE(op_present);

    // Check if s1 s3 is in the first two operators
    op_present = false;
    exp_op = {1,0,1};
    for (int i = 0; i < 2; ++i){
        for (int j = 0; j < 3; ++j){
            op.push_back(spin_ops[j][i]);
        }
        if (op == exp_op){
            op_present = true;
        }
        op.clear();
    }
    EXPECT_TRUE(op_present);

    // Next three operators should be s1, s3, s1s2
    op_present = false;
    exp_op = {1,0,0};
    for (int i = 2; i < 5; ++i){
        for (int j = 0; j < 3; ++j){
            op.push_back(spin_ops[j][i]);
        }
        if (op == exp_op){
            op_present = true;
        }
        op.clear();
    }
    EXPECT_TRUE(op_present);

    op_present = false;
    exp_op = {0,0,1};
    for (int i = 2; i < 5; ++i){
        for (int j = 0; j < 3; ++j){
            op.push_back(spin_ops[j][i]);
        }
        if (op == exp_op){
            op_present = true;
        }
        op.clear();
    }
    EXPECT_TRUE(op_present);

    op_present = false;
    exp_op = {1,1,0};
    for (int i = 2; i < 5; ++i){
        for (int j = 0; j < 3; ++j){
            op.push_back(spin_ops[j][i]);
        }
        if (op == exp_op){
            op_present = true;
        }
        op.clear();
    }
    EXPECT_TRUE(op_present);

    // Next two operators should be s1^2 s2, s1^2 s3
    op_present = false;
    exp_op = {2,1,0};
    for (int i = 5; i < 7; ++i){
        for (int j = 0; j < 3; ++j){
            op.push_back(spin_ops[j][i]);
        }
        if (op == exp_op){
            op_present = true;
        }
        op.clear();
    }
    EXPECT_TRUE(op_present);

    op_present = false;
    exp_op = {2,0,1};
    for (int i = 5; i < 7; ++i){
        for (int j = 0; j < 3; ++j){
            op.push_back(spin_ops[j][i]);
        }
        if (op == exp_op){
            op_present = true;
        }
        op.clear();
    }
    EXPECT_TRUE(op_present);    

    // Last two operators should be s2 s3, s2^2 s3
    op_present = false;
    exp_op = {0,1,1};
    for (int i = 7; i < 9; ++i){
        for (int j = 0; j < 3; ++j){
            op.push_back(spin_ops[j][i]);
        }
        if (op == exp_op){
            op_present = true;
        }
        op.clear();
    }
    EXPECT_TRUE(op_present);

    op_present = false;
    exp_op = {0,2,1};
    for (int i = 7; i < 9; ++i){
        for (int j = 0; j < 3; ++j){
            op.push_back(spin_ops[j][i]);
        }
        if (op == exp_op){
            op_present = true;
        }
        op.clear();
    }
    EXPECT_TRUE(op_present);

    
    // Sorting all operators
    spin_ops.clear();
    sort_spin_ops(data, spin_ops, n);

    // There should be 13 operators
    EXPECT_EQ(spin_ops.size(), n);
    EXPECT_EQ(spin_ops[0].size(), 13);

    // First operator should be s1, s2^2, s3
    exp_op = {1,2,1};
    op = {spin_ops[0][0], spin_ops[1][0], spin_ops[2][0]};
    EXPECT_EQ(op, exp_op);
    op.clear();

    // Next three operators should be s2, s1 s3, s1 s2 s3
    op_present = false;
    exp_op = {0,1,0};
    for (int i = 1; i < 4; ++i){
        for (int j = 0; j < 3; ++j){
            op.push_back(spin_ops[j][i]);
        }
        if (op == exp_op){
            op_present = true;
        }
        op.clear();
    }
    EXPECT_TRUE(op_present);

    op_present = false;
    exp_op = {1,0,1};
    for (int i = 1; i < 4; ++i){
        for (int j = 0; j < 3; ++j){
            op.push_back(spin_ops[j][i]);
        }
        if (op == exp_op){
            op_present = true;
        }
        op.clear();
    }
    EXPECT_TRUE(op_present);

    op_present = false;
    exp_op = {1,1,1};
    for (int i = 1; i < 4; ++i){
        for (int j = 0; j < 3; ++j){
            op.push_back(spin_ops[j][i]);
        }
        if (op == exp_op){
            op_present = true;
        }
        op.clear();
    }
    EXPECT_TRUE(op_present);    
    
    // Next three operators should be s1, s3, s1s2
    op_present = false;
    exp_op = {1,0,0};
    for (int i = 4; i < 7; ++i){
        for (int j = 0; j < 3; ++j){
            op.push_back(spin_ops[j][i]);
        }
        if (op == exp_op){
            op_present = true;
        }
        op.clear();
    }
    EXPECT_TRUE(op_present);

    op_present = false;
    exp_op = {0,0,1};
    for (int i = 4; i < 7; ++i){
        for (int j = 0; j < 3; ++j){
            op.push_back(spin_ops[j][i]);
        }
        if (op == exp_op){
            op_present = true;
        }
        op.clear();
    }
    EXPECT_TRUE(op_present);

    op_present = false;
    exp_op = {1,1,0};
    for (int i = 4; i < 7; ++i){
        for (int j = 0; j < 3; ++j){
            op.push_back(spin_ops[j][i]);
        }
        if (op == exp_op){
            op_present = true;
        }
        op.clear();
    }
    EXPECT_TRUE(op_present);

    // Second dataset

    // Variables
    n = 2;
    q = 4;
    // Create data object
    data = Data("../tests/test_basis_search.dat", n, q);

    // Sorting all operators
    spin_ops.clear();
    sort_spin_ops(data, spin_ops, n);

    EXPECT_EQ(spin_ops.size(), n);
    EXPECT_EQ(spin_ops[0].size(), 6);

    // Order of the first three operators should be s1, s1 s2^2, s1 s2
    exp_op = {1,0};
    op = {spin_ops[0][0], spin_ops[1][0]};
    EXPECT_EQ(op, exp_op);

    exp_op = {1,2};
    op = {spin_ops[0][1], spin_ops[1][1]};
    EXPECT_EQ(op, exp_op);

    exp_op = {1,1};
    op = {spin_ops[0][2], spin_ops[1][2]};
    EXPECT_EQ(op, exp_op);
}