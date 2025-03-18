#pragma once

#include "data/dataset.h"
#include "model/model.h"
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
     * Return the initial model.
     */
    Model get_model_in();

    /**
     * Return the final model.
     */
    Model get_model_out();

    // Search methods
    Model exhaustive_search(Data& data);
    Model greedy_search(Data& data, Model* init_model = nullptr);
    Model divide_and_conquer(Data& data, Model* init_model = nullptr);
    Model simulated_annealing(Data& data, Model* init_model = nullptr);

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
    Model model_in;
    Model model_out;
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

    int division(int move_from, int move_to);

    int merge_partition(std::vector<__uint128_t>& partition, SA_settings& settings);
    int split_partition(std::vector<__uint128_t>& partition, SA_settings& settings);
    int switch_partition(std::vector<__uint128_t>& partition, SA_settings& settings);

    double get_log_ev(std::vector<__uint128_t> partition);
    double get_log_ev_icc(__uint128_t component);
};