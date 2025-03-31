#pragma once

#include "data/dataset.h"
#include "model/mcm.h"
#include "search/annealing.h"

#include "utilities/miscellaneous.h"
#include "utilities/partition.h"

#include <random>

class MCMSearch {
public:
    /**
     * Constructs an MCMSearch object that contains several methods to find the best partition.
     */
    MCMSearch();

    /**
     * Return the initial mcm.
     */
    MCM get_mcm_in();

    /**
     * Return the final mcm.
     */
    MCM get_mcm_out();

    // Search methods
    MCM exhaustive_search(Data& data);
    MCM greedy_search(Data& data, MCM* init_mcm = nullptr);
    MCM divide_and_conquer(Data& data, MCM* init_mcm = nullptr);
    MCM simulated_annealing(Data& data, MCM* init_mcm = nullptr);

    // Setters and getters for the simulated annealing settings
    void set_SA_max_iter(int n_iter);
    void set_SA_init_temp(int temp);
    void set_SA_update_schedule(int n_iter);
    int get_SA_max_iter() {return this->SA_max_iter;};
    int get_SA_init_temp() {return this->SA_T0;};
    int get_SA_update_schedule() {return this->SA_update_schedule;};

    // Getter for all evidences in exhaustive search
    std::vector<double> get_log_evidence_trajectory();


private:
    MCM mcm_in;
    MCM mcm_out;
    Data* data = nullptr;
    
    std::string log_file;

    int SA_max_iter;
    int SA_T0;
    int SA_update_schedule;

    std::map<__uint128_t, double> evidence_storage;
    std::vector<double> evidence_storage_es;

    std::vector<double> all_evidences;
    std::vector<double> log_evidence_trajectory;

    bool exhaustive;

    // Divide and conquer function
    int division(int move_from, int move_to);

    // Simulated annealing functions
    int merge_partition(MCM& mcm, SA_settings& settings);
    int split_partition(MCM& mcm, SA_settings& settings);
    int switch_partition(MCM& mcm, SA_settings& settings);

    // Greedy merging function
    void hierarchical_merging();

    double get_log_ev(std::vector<__uint128_t> partition);
    double get_log_ev_icc(__uint128_t component);
};