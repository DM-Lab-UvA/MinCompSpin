#include "gtest/gtest.h"
#include "search/basis_search/basis_search.h"

TEST(basis_search, up_to_order_k){
    // Assign variables
    int n = 2;
    int q = 4;

    // Create dataset
    Data data("../tests/test_basis_search.dat", n, q);
    // Create search object
    BasisSearch searcher = BasisSearch();

    Basis basis = searcher.up_to_order_k(data, n);

    // Expected basis (s1, s1 s2)
    std::vector<std::vector<uint8_t>> basis_matrix = {{1,1}, {0,1}};
    std::vector<std::vector<__uint128_t>> basis_ops = {{1,0}, {3,0}};

    EXPECT_EQ(basis.get_n(), n);
    EXPECT_EQ(basis.get_q(), q);
    EXPECT_EQ(basis.get_basis_matrix(), basis_matrix);
    EXPECT_EQ(basis.get_basis(), basis_ops);

    Basis basis2 = searcher.up_to_order_k(data, 1);

    // Expected basis (s1, s2)
    basis_matrix = {{1,0}, {0,1}};
    basis_ops = {{1,0}, {2,0}};

    EXPECT_EQ(basis2.get_n(), n);
    EXPECT_EQ(basis2.get_q(), q);
    EXPECT_EQ(basis2.get_basis_matrix(), basis_matrix);
    EXPECT_EQ(basis2.get_basis(), basis_ops);  
    
    // TEST dataset
    n = 3;
    q = 3;

    data = Data("../tests/test.dat", n, q);
    basis = searcher.up_to_order_k(data, 2);

    // Check the entropy of the three best operators
    EXPECT_FLOAT_EQ(calc_entropy_of_spin_op(data, basis.get_basis()[0]), 0.7963116401738131);
    EXPECT_FLOAT_EQ(calc_entropy_of_spin_op(data, basis.get_basis()[1]), 0.7963116401738131);
    EXPECT_FLOAT_EQ(calc_entropy_of_spin_op(data, basis.get_basis()[2]), 0.9556998911125343);
}

TEST(basis_search, exhaustive){
    // Should be the same as up to order n
    // Assign variables
    int n = 2;
    int q = 4;

    // Create dataset
    Data data("../tests/test_basis_search.dat", n, q);
    // Create search object
    BasisSearch searcher = BasisSearch();

    Basis basis = searcher.exhaustive_search(data);
    Basis basis2 = searcher.up_to_order_k(data, n); 
    
    EXPECT_EQ(basis2.get_n(), basis.get_n());
    EXPECT_EQ(basis2.get_q(), basis.get_q());
    EXPECT_EQ(basis2.get_basis_matrix(), basis.get_basis_matrix());
    EXPECT_EQ(basis2.get_basis(), basis.get_basis());

    // TEST dataset
    n = 3;
    q = 3;

    data = Data("../tests/test.dat", n, q);
    basis = searcher.exhaustive_search(data);

    // Check the first operator of the basis
    // Should be s1 s2^2 s3
    std::vector<__uint128_t> op = {5,2};
    EXPECT_EQ(basis.get_basis()[0], op);

    // Check the entropy of the three best operators
    EXPECT_FLOAT_EQ(calc_entropy_of_spin_op(data, basis.get_basis()[0]), 0.6829081047004717);
    EXPECT_FLOAT_EQ(calc_entropy_of_spin_op(data, basis.get_basis()[1]), 0.7963116401738131);
    EXPECT_FLOAT_EQ(calc_entropy_of_spin_op(data, basis.get_basis()[2]), 0.9556998911125343);
}

TEST(basis_search, iterative){
    // Assign variables
    int n = 3;
    int q = 3;

    // Create data object
    Data data("../tests/test.dat", n, q);
    // Create search object
    BasisSearch searcher = BasisSearch();

    Basis basis = searcher.iterative_search(data, 2);

    // Check the first operator of the basis
    // Should be s1^2 s2 s3^2
    std::vector<__uint128_t> op = {2,5};
    EXPECT_EQ(basis.get_basis()[0], op);

    // Check the entropy of the three best operators
    EXPECT_FLOAT_EQ(calc_entropy_of_spin_op(data, basis.get_basis()[0]), 0.6829081047004717);
    EXPECT_FLOAT_EQ(calc_entropy_of_spin_op(data, basis.get_basis()[1]), 0.7963116401738131);
    EXPECT_FLOAT_EQ(calc_entropy_of_spin_op(data, basis.get_basis()[2]), 0.9556998911125343);
}