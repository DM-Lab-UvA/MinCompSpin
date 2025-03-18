#include "gtest/gtest.h"
#include "../../include/search/search.h"

TEST(search, init_n) {
    // Initialize
    MCMSearch searcher = MCMSearch();

    try {
        searcher.get_model_in();
        FAIL() << "Expected std::runtime_error";
    }
    catch(std::runtime_error const & err) {
        EXPECT_EQ(err.what(), std::string("No search has been ran yet."));
    }

    try {
        searcher.get_model_out();
        FAIL() << "Expected std::runtime_error";
    }
    catch(std::runtime_error const & err) {
        EXPECT_EQ(err.what(), std::string("No search has been ran yet."));
    }

    EXPECT_EQ(searcher.get_SA_max_iter(), 50000);
    EXPECT_EQ(searcher.get_SA_init_temp(), 100);
}

TEST(search, SA_params) {
    // Initialize MCMSearch object
    MCMSearch searcher = MCMSearch();

    // Set the parameters
    int n_iter = 500;
    int temp = 200;
    int update_schedule = 400;

    try {
        searcher.set_SA_max_iter(0);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("The maximum number of iterations should be a positive number."));
    }

    try {
        searcher.set_SA_init_temp(0);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("The initial annealing temperature should be positive."));
    }

    try {
        searcher.set_SA_update_schedule(0);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("The number of iterations before updating the temperature should be a positive number."));
    }

    searcher.set_SA_max_iter(n_iter);
    EXPECT_EQ(searcher.get_SA_max_iter(), n_iter);
    searcher.set_SA_init_temp(temp);
    EXPECT_EQ(searcher.get_SA_init_temp(), temp);
    searcher.set_SA_update_schedule(update_schedule);
    EXPECT_EQ(searcher.get_SA_update_schedule(), update_schedule);   
}

TEST(search, exhaustive){
    // Initialize dataset
    int n = 3;
    Data data("../tests/test.dat", 3, 3);
    // Initialize MCMSearch object
    MCMSearch searcher = MCMSearch();

    searcher.exhaustive_search(data);

    try {
        searcher.get_model_in();
        FAIL() << "Expected std::runtime_error";
    }
    catch(std::runtime_error const & err) {
        EXPECT_EQ(err.what(), std::string("Exhaustive search does not have an initial model."));
    }

    Model mcm = searcher.get_model_out();

    // Expected results
    std::vector<__uint128_t> partition = {7,0,0};
    double evidence = -23.324842793537613;
    std::vector<double> evidence_per_icc = {-23.324842793537613, 0, 0};
    
    EXPECT_EQ(mcm.n, n);
    EXPECT_EQ(mcm.n_comp, 1);
    EXPECT_EQ(partition, mcm.partition);
    EXPECT_FLOAT_EQ(mcm.get_best_log_ev(), evidence);
    EXPECT_EQ(mcm.get_best_log_ev_per_icc().size(), 3);
    EXPECT_FLOAT_EQ(mcm.get_best_log_ev_per_icc()[0], evidence);
    EXPECT_EQ(mcm.get_best_log_ev_per_icc()[1], 0);
    EXPECT_EQ(mcm.get_best_log_ev_per_icc()[2], 0);
    EXPECT_TRUE(mcm.optimized);

    std::vector<double> all_evs(5, 0);
    all_evs[0] = data.calc_log_ev_icc(7);
    all_evs[1] =  data.calc_log_ev_icc(4) + data.calc_log_ev_icc(3);
    all_evs[2] = data.calc_log_ev_icc(5) + data.calc_log_ev_icc(2);
    all_evs[3] = data.calc_log_ev_icc(6) + data.calc_log_ev_icc(1);
    all_evs[4] =  data.calc_log_ev_icc(1) + data.calc_log_ev_icc(2) + data.calc_log_ev_icc(4);

    EXPECT_EQ(searcher.get_log_evidence_trajectory().size(), 5);
    EXPECT_EQ(all_evs, searcher.get_log_evidence_trajectory());
}

TEST(search, greedy) {
    // Initialize dataset
    int n = 3;
    Data data("../tests/test.dat", 3, 3);
    // Initialize MCMSearch object
    MCMSearch searcher = MCMSearch();

    searcher.greedy_search(data);
    Model mcm_in = searcher.get_model_in();
    Model mcm_out = searcher.get_model_out();

    // Expected results
    std::vector<__uint128_t> indep_partition = {1,2,4};
    std::vector<__uint128_t> partition = {7,0,0};
    double evidence = -23.324842793537613;
    std::vector<double> evidence_per_icc = {-23.324842793537613, 0, 0};

    EXPECT_EQ(mcm_in.n, n);
    EXPECT_EQ(mcm_in.n_comp, n);
    EXPECT_EQ(indep_partition, mcm_in.partition);
    EXPECT_FALSE(mcm_in.optimized);

    try {
        mcm_in.get_best_log_ev();
        FAIL() << "Expected std::runtime_error";
    }
    catch(std::runtime_error const & err) {
        EXPECT_EQ(err.what(), std::string("No search has been ran yet."));
    }

    EXPECT_EQ(mcm_out.n, n);
    EXPECT_EQ(mcm_out.n_comp, 1);
    EXPECT_EQ(partition, mcm_out.partition);
    EXPECT_FLOAT_EQ(mcm_out.get_best_log_ev(), evidence);
    EXPECT_EQ(mcm_out.get_best_log_ev_per_icc().size(), 3);
    EXPECT_FLOAT_EQ(mcm_out.get_best_log_ev_per_icc()[0], evidence);
    EXPECT_EQ(mcm_out.get_best_log_ev_per_icc()[1], 0);
    EXPECT_EQ(mcm_out.get_best_log_ev_per_icc()[2], 0);
    EXPECT_TRUE(mcm_out.optimized);

    // Wrong number of variables in the initial model
    mcm_in = Model(4, "independent");
    try {
        searcher.greedy_search(data, &mcm_in);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("Number of variables in the data doesn't match the number of variables in the given model."));
    }
    
    // Start search from complete model
    mcm_in = Model(3, "complete");
    searcher.greedy_search(data, &mcm_in);
    mcm_out = searcher.get_model_out();
    Model init_mcm = searcher.get_model_in();

    // Expected results
    EXPECT_EQ(mcm_in.n, init_mcm.n);
    EXPECT_EQ(mcm_in.n_comp, init_mcm.n_comp);
    EXPECT_EQ(mcm_in.partition, init_mcm.partition);
    EXPECT_FALSE(init_mcm.optimized);

    EXPECT_EQ(mcm_out.n, n);
    EXPECT_EQ(mcm_out.n_comp, 1);
    EXPECT_EQ(partition, mcm_out.partition);
    EXPECT_FLOAT_EQ(mcm_out.get_best_log_ev(), evidence);
    EXPECT_EQ(mcm_out.get_best_log_ev_per_icc().size(), 3);
    EXPECT_FLOAT_EQ(mcm_out.get_best_log_ev_per_icc()[0], evidence);
    EXPECT_EQ(mcm_out.get_best_log_ev_per_icc()[1], 0);
    EXPECT_EQ(mcm_out.get_best_log_ev_per_icc()[2], 0); 
    EXPECT_TRUE(mcm_out.optimized);   
}

TEST(search, div_and_conq) {
    // Initialize dataset
    int n = 3;
    Data data("../tests/test.dat", 3, 3);
    // Initialize MCMSearch object
    MCMSearch searcher = MCMSearch();

    searcher.divide_and_conquer(data);
    Model mcm_in = searcher.get_model_in();
    Model mcm_out = searcher.get_model_out();

    // Expected results
    std::vector<__uint128_t> complete_partition = {7,0,0};
    std::vector<__uint128_t> partition = {7,0,0};
    double evidence = -23.324842793537613;
    std::vector<double> evidence_per_icc = {-23.324842793537613, 0, 0};

    EXPECT_EQ(mcm_in.n, n);
    EXPECT_EQ(mcm_in.n_comp, 1);
    EXPECT_EQ(complete_partition, mcm_in.partition);
    EXPECT_FALSE(mcm_in.optimized);

    EXPECT_EQ(mcm_out.n, n);
    EXPECT_EQ(mcm_out.n_comp, 1);
    EXPECT_EQ(partition, mcm_out.partition);
    EXPECT_FLOAT_EQ(mcm_out.get_best_log_ev(), evidence);
    EXPECT_EQ(mcm_out.get_best_log_ev_per_icc().size(), 3);
    EXPECT_FLOAT_EQ(mcm_out.get_best_log_ev_per_icc()[0], evidence);
    EXPECT_EQ(mcm_out.get_best_log_ev_per_icc()[1], 0);
    EXPECT_EQ(mcm_out.get_best_log_ev_per_icc()[2], 0);
    EXPECT_TRUE(mcm_out.optimized);

    // Wrong number of variables in the initial model
    mcm_in = Model(4, "independent");
    try {
        searcher.divide_and_conquer(data, &mcm_in);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("Number of variables in the data doesn't match the number of variables in the given model."));
    }

    
    // Start from independent model
    mcm_in = Model(n, "independent");
    searcher.divide_and_conquer(data, &mcm_in);
    mcm_out = searcher.get_model_out();
    Model init_mcm = searcher.get_model_in();

    std::vector<__uint128_t> indep_partition = {1,2,4};
    evidence_per_icc[0] = data.calc_log_ev_icc(1);
    evidence_per_icc[1] = data.calc_log_ev_icc(2);
    evidence_per_icc[2] = data.calc_log_ev_icc(4);

    EXPECT_EQ(mcm_in.n, init_mcm.n);
    EXPECT_EQ(mcm_in.n_comp, init_mcm.n_comp);
    EXPECT_EQ(mcm_in.partition, init_mcm.partition);
    EXPECT_FALSE(init_mcm.optimized);

    EXPECT_EQ(mcm_out.n, n);
    EXPECT_EQ(mcm_out.n_comp, 3);
    EXPECT_EQ(indep_partition, mcm_out.partition);
    EXPECT_EQ(mcm_out.get_best_log_ev(), data.calc_log_ev_icc(1) + data.calc_log_ev_icc(2) + data.calc_log_ev_icc(4));
    EXPECT_EQ(evidence_per_icc, mcm_out.get_best_log_ev_per_icc());
    EXPECT_TRUE(mcm_out.optimized);
}

TEST(search, sim_annealing) {
    srand(0);
    // Initialize dataset
    int n = 3;
    Data data("../tests/test.dat", 3, 3);
    // Initialize MCMSearch object
    MCMSearch searcher = MCMSearch();

    searcher.simulated_annealing(data);
    Model mcm_in = searcher.get_model_in();
    Model mcm_out = searcher.get_model_out();

    // Expected results
    std::vector<__uint128_t> partition = {7,0,0};
    double evidence = -23.324842793537613;
    
    EXPECT_EQ(mcm_out.n, n);
    EXPECT_EQ(mcm_out.n_comp, 1);
    EXPECT_EQ(partition, mcm_out.partition);
    EXPECT_FLOAT_EQ(mcm_out.get_best_log_ev(), evidence);
    EXPECT_EQ(mcm_out.get_best_log_ev_per_icc().size(), 3);
    EXPECT_FLOAT_EQ(mcm_out.get_best_log_ev_per_icc()[0], 0);
    EXPECT_EQ(mcm_out.get_best_log_ev_per_icc()[1], 0);
    EXPECT_FLOAT_EQ(mcm_out.get_best_log_ev_per_icc()[2], -23.324842793537613);
    EXPECT_TRUE(mcm_out.optimized);

    // Wrong number of variables in the initial model
    mcm_in = Model(4, "independent");
    try {
        searcher.simulated_annealing(data, &mcm_in);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(), std::string("Number of variables in the data doesn't match the number of variables in the given model."));
    }
}

