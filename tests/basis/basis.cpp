#include "gtest/gtest.h"
#include "basis/basis.h"

TEST(basis, init_n){
    // Declare variables
    int n;
    int q;

    std::vector<std::vector<__uint128_t>> spin_ops;
    std::vector<std::vector<__uint128_t>> exp_spin_ops;

    std::vector<std::vector<uint8_t>> spin_ops_array;
    std::vector<std::vector<uint8_t>> exp_spin_ops_array;

    // Invalid number of variables
    n = 0;
    try {
        Basis basis(n, q);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("The number of variables should be a non-zero positive number."));
    }

    // Too many variables
    n = 129;
    try {
        Basis basis(n, q);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("The maximum system size is 128 variables."));
    }

    // Invalid value for q
    n = 3;
    q = 1;
    try {
        Basis basis(n, q);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("The number of states should be at least 2."));
    }    

    
    // Create Basis object with default basis
    q = 5;
    Basis basis(n, q);

    exp_spin_ops = {{1,0,0}, {2,0,0}, {4,0,0}};
    spin_ops = basis.get_basis();

    exp_spin_ops_array = {{1,0,0}, {0,1,0}, {0,0,1}};
    spin_ops_array = basis.get_basis_matrix();

    EXPECT_EQ(exp_spin_ops, spin_ops);
    EXPECT_EQ(exp_spin_ops_array, spin_ops_array);
    EXPECT_EQ(basis.get_n(), n);
    EXPECT_EQ(basis.get_q(), q);
    EXPECT_EQ(basis.get_n_ints(), 3);
}

TEST(basis, init_spin_ops){
    // Declare variables
    int n;
    int q;

    std::vector<std::vector<__uint128_t>> spin_ops;
    std::vector<std::vector<__uint128_t>> exp_spin_ops;

    std::vector<std::vector<uint8_t>> spin_ops_array;
    std::vector<std::vector<uint8_t>> exp_matrix;

    // Invalid number of variables
    n = 0;
    try {
        Basis basis(n, q, spin_ops_array);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("The number of variables should be a non-zero positive number."));
    }

    // Too many variables
    n = 129;
    try {
        Basis basis(n, q, spin_ops_array);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("The maximum system size is 128 variables."));
    }

    // Invalid value for q
    n = 3;
    q = 1;
    try {
        Basis basis(n, q, spin_ops_array);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("The number of states should be at least 2."));
    }
    
    // Initialize with too many spin ops
    n = 4;
    q = 3;
    spin_ops_array = {{1,0,0,0}, {0,2,0,0}, {0,1,2,0}, {1,0,2,1}, {0,0,1,1}};
    try {
        Basis basis(n, q, spin_ops_array);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("The given basis doesn't have n spin operators."));
    }

    // Initialize with too few spin ops
    spin_ops_array = {{1,0,0,0}, {0,2,0,0}};
    try {
        Basis basis(n, q, spin_ops_array);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("The given basis doesn't have n spin operators."));
    }

    // Initialize with invalid number of entries for spin op
    spin_ops_array = {{1,0,0,0,0}, {0,2,0,0,1}, {0,1,2,0,1}, {1,0,2,1,0}};
    try {
        Basis basis(n, q, spin_ops_array);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("Invalid spin operator in the given basis because it does not contain n variables."));
    }

    spin_ops_array = {{1,0}, {0,2}, {0,1}, {1,0}};
    try {
        Basis basis(n, q, spin_ops_array);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("Invalid spin operator in the given basis because it does not contain n variables."));
    }

    // Initialize with variables raised to a power larger than q-1
    spin_ops_array = {{1,0,0,0}, {0,2,0,0}, {0,1,2,3}, {1,0,2,1}};
    try {
        Basis basis(n, q, spin_ops_array);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("The vector should only contain values between 0 and q-1."));
    }
    
    // Initialize with linearly dependent basis
    spin_ops_array = {{1,2,0,0}, {2,1,0,0}, {0,0,1,0}, {0,0,2,1}};
    try {
        Basis basis(n, q, spin_ops_array);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("Given basis is not linearly independent."));
    }

    // Initialize with valid basis
    spin_ops_array = {{1,2,0,0}, {2,2,0,0}, {0,0,1,0}, {0,0,2,1}};
    exp_matrix = {{1,2,0,0}, {2,2,0,0}, {0,0,1,2}, {0,0,0,1}};
    Basis basis(n, q, spin_ops_array);

    spin_ops = basis.get_basis();
    exp_spin_ops = {{1,2}, {0,3}, {4,0}, {8,4}};

    EXPECT_EQ(exp_spin_ops, spin_ops);
    EXPECT_EQ(basis.get_basis_matrix(), exp_matrix);
    EXPECT_EQ(basis.get_n(), n);
    EXPECT_EQ(basis.get_q(), q);
    EXPECT_EQ(basis.get_n_ints(), 2);

    // Invalid for q = 4 because linearly dependent due to s1^2 s2^2
    try {
        Basis basis(n, 4, spin_ops_array);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("Given basis is not linearly independent."));
    }
}

TEST(basis, init_file){
    // Declare variables
    int q;
    int n;
    std::string filename;
    std::vector<std::vector<__uint128_t>> spin_ops;
    std::vector<std::vector<__uint128_t>> exp_spin_ops;
    std::vector<std::vector<uint8_t>> exp_spin_ops_array;

    // Invalid number of variables
    n = 0;
    try {
        Basis basis(n, q, filename);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("The number of variables should be a non-zero positive number."));
    }

    // Too many variables
    n = 129;
    try {
        Basis basis(n, q, filename);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("The maximum system size is 128 variables."));
    }

    // Invalid value for q
    n = 3;
    q = 1;
    try {
        Basis basis(n, q, filename);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("The number of states should be at least 2."));
    }

    // Wrong filename
    q = 3;
    filename = "non_existing_file.dat";
    try {
        Basis basis(n, q, filename);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("Not able to open the file."));
    }

    // Read in file with too many spin operators
    filename = "../tests/test.dat";
    try {
        Basis basis(n, q, filename);
        FAIL () << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("File contains too many spin operators."));
    }

    // Read in file with invalid number of entries for spin op
    n = 7;
    try {
        Basis basis(n, q, filename);
        FAIL () << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("File contains invalid spin operator because it has less than n variables."));
    }

    // Read in with variables raised to a power larger than q-1
    n = 4;
    q = 2;
    filename = "../tests/basis2.dat";
    try {
        Basis basis(n, q, filename);
        FAIL () << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("Entries in the file should only contain values between 0 and q-1."));
    } 

    // Read in with linearly dependent basis
    q = 4;
    try {
        Basis basis(n, q, filename);
        FAIL () << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("Given basis is not linearly independent."));
    }

    // Read in with valid basis
    q = 3;
    Basis basis(n, q, filename);

    spin_ops = basis.get_basis();
    exp_spin_ops = {{1,2}, {0,3}, {4,0}, {8,4}};
    exp_spin_ops_array = {{1,2,0,0}, {2,2,0,0}, {0,0,1,2}, {0,0,0,1}};

    EXPECT_EQ(exp_spin_ops, spin_ops);
    EXPECT_EQ(basis.get_basis_matrix(), exp_spin_ops_array);
    EXPECT_EQ(basis.get_n(), n);
    EXPECT_EQ(basis.get_q(), q);
    EXPECT_EQ(basis.get_n_ints(), 2);
}