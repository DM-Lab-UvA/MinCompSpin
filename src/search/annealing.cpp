#include "search/search.h"
#include "search/annealing.h"

Model MCMSearch::simulated_annealing(Data& data, Model* init_model){
    int n = data.n;
    this->data = &data;
    // Initialize a model to store the result
    if(!init_model){
        // Default initial model is random one
        this->model_in = Model(n, "random");
        this->model_out = Model(n, this->model_in.get_partition());
    }
    else{
        // Check if the number of variables in the data and the model match
        if (n != init_model->n) {
            throw std::invalid_argument("Number of variables in the data doesn't match the number of variables in the given model.");
        }
        this->model_in = *init_model;
        this->model_out = *init_model;
    }

    // Make a hard copy of the initial partition
    std::vector<__uint128_t> partition = this->model_out.partition;
    this->model_out.log_ev = this->get_log_ev(partition);

    // Clear from previous search
    this->log_evidence_trajectory.clear();
    this->exhaustive = false;

    // Initialize a struct containing the SA settings
    SA_settings settings(this->SA_T0, this->model_out.partition);

    int x;
    int accepted;
    double log_ev;
    int steps_since_improve = 0;

    for (int i = 0; i < this->SA_max_iter; i++){
        if (this->model_out.n_comp == data.n){
            x = 0;
        }
        else if (this->model_out.n_comp == 1){
            x = 1;
        }
        else{
            x = rand()/(RAND_MAX/3);
        }

        if (x == 0){
            accepted = this->merge_partition(partition, settings);
        }
        else if (x == 1){
            accepted = this->split_partition(partition, settings);
        }
        else{
            accepted = this->switch_partition(partition, settings);
        }

        // Update the temperature
        if (i % this->SA_update_schedule == 0){
            settings.temp = this->SA_T0 / (1 + log(1 + i));
        }

        // Update solution if log evidence improved
        log_ev = this->get_log_ev(partition);
        if (log_ev > this->model_out.log_ev && fabs(log_ev - this->model_out.log_ev) > settings.epsilon){
            this->model_out.partition = partition;
            this->model_out.log_ev = log_ev;

            steps_since_improve = 0;
            std::cout << "best log-evidence: " << log_ev << "\t@T = " << settings.temp << "\ti = " << i << std::endl;
        }
        else{steps_since_improve++;}
        this->log_evidence_trajectory.push_back(this->model_out.log_ev);

        // Check stopping criteria
        if (steps_since_improve > settings.max_no_improve){
            std::cout << "\n- maximum iterations without improvement reached" << std::endl;
            break;}
    }
    // Calculate the log ev per icc
    this->model_out.log_ev_per_icc.assign(n, 0);
    this->model_out.n_comp = 0;
    for (int i = 0; i < n; i++){
        if (this->model_out.partition[i]){
            this->model_out.log_ev_per_icc[i] = this->get_log_ev_icc(this->model_out.partition[i]);
            this->model_out.n_comp++;
        }
    }
    place_empty_component_last(this->model_out.partition);
    // Indicate that the search has been done
    this->model_out.optimized = true;

    return this->model_out;
}

int MCMSearch::merge_partition(std::vector<__uint128_t>& partition, SA_settings& settings){
    if (this->model_out.n_comp <= 1){return 2;}

    __uint128_t ONE = 1;

    // Choose two random components
    int comp_1 = randomBitIndex(settings.occupied_comp);
    int comp_2 = randomBitIndex(settings.occupied_comp - (ONE << comp_1));

    // Calculate the change in evidence when merging
    __uint128_t merged_comp = partition[comp_1] + partition[comp_2];
    double merged_log_ev = this->get_log_ev_icc(merged_comp);
    double diff_log_ev = merged_log_ev - this->get_log_ev_icc(partition[comp_1]) - this->get_log_ev_icc(partition[comp_2]);

    // Check if new partition is accepted using metropolis acceptance probability
    double p = exp(diff_log_ev / settings.temp);
    double u = ((double) rand() / (RAND_MAX));

    if (p > u){
        // Accept the new partition
        partition[comp_1] = merged_comp;
        partition[comp_2] = 0;
        
        settings.occupied_comp -= (ONE << comp_2);
        if ((settings.occupied_comp2 & (ONE << comp_1)) == 0){
            settings.occupied_comp2 += (ONE << comp_1);
        }
        if ((settings.occupied_comp2 & (ONE << comp_2)) == (ONE << comp_2)){
            settings.occupied_comp2 -= (ONE << comp_2);
        }
        this->model_out.n_comp--;

        return 0;

    }
    return 1;
}

int MCMSearch::split_partition(std::vector<__uint128_t>& partition, SA_settings& settings){
    if (this->model_out.n_comp == this->data->n){return 2;}
    if (settings.occupied_comp2 == 0){return 2;}

    __uint128_t ONE = 1;

    // Choose random component containing at least two variables
    int comp_index = randomBitIndex(settings.occupied_comp2);
    __uint128_t comp = partition[comp_index];

    // Make a random split of the component
    __uint128_t mask = random_128_int(this->data->n);
    __uint128_t comp_1 = (comp & mask);
    __uint128_t comp_2 = (comp & (~mask));

    // Both component should at least contain 1 variable
    while ((bit_count(comp_1) == 0) || (bit_count(comp_2) == 0)){
        mask = random_128_int(this->data->n);
        comp_1 = (comp & mask);
        comp_2 = (comp & (~mask));
    }

    // Calculate the change in evidence when splitting
    double log_ev_1 = this->get_log_ev_icc(comp_1);
    double log_ev_2 = this->get_log_ev_icc(comp_2);
    double diff_log_ev = log_ev_1 + log_ev_2 - this->get_log_ev_icc(comp);

    // Check if new partition is accepted using metropolis acceptance probability
    double p = exp(diff_log_ev / settings.temp);
    double u = ((double) rand() / (RAND_MAX));

    if (p > u){
        // Accept the new partition
        partition[comp_index] = comp_1;
        // Find the first empty component
        int comp_2_index = 0;
        while (partition[comp_2_index]){comp_2_index++;}
        partition[comp_2_index] = comp_2;

        settings.occupied_comp += (ONE << comp_2_index);
        if (bit_count(comp_1) <= 1){
            settings.occupied_comp2 -= (ONE << comp_index);
        }
        if (bit_count(comp_2) > 1){
            settings.occupied_comp2 += (ONE << comp_2_index);
        }
        this->model_out.n_comp++;

        return 0;
    }
    return 1;
}

int MCMSearch::switch_partition(std::vector<__uint128_t>& partition, SA_settings& settings){
    if (this->model_out.n_comp == 1 || this->model_out.n_comp == this->data->n){return 2;}
    if (settings.occupied_comp2 == 0){return 2;}

    __uint128_t ONE = 1;

    // Select two random partitions, the first with at least two variables
    int comp_1_index = randomBitIndex(settings.occupied_comp2);
    int comp_2_index = randomBitIndex(settings.occupied_comp - (ONE << comp_1_index));
    __uint128_t comp_1 = partition[comp_1_index];
    __uint128_t comp_2 = partition[comp_2_index];

    // Select random variable from first component
    int var = randomBitIndex(comp_1);
    // Move the variable from component 1 to component 2
    __uint128_t new_comp_1 = comp_1 - (ONE << var);
    __uint128_t new_comp_2 = comp_2 + (ONE << var);

    // Calculate the difference in evidence when moving the variable
    double log_ev_1 = this->get_log_ev_icc(comp_1);
    double log_ev_2 = this->get_log_ev_icc(comp_2);
    double new_log_ev_1 = this->get_log_ev_icc(new_comp_1);
    double new_log_ev_2 = this->get_log_ev_icc(new_comp_2);
    double diff_log_ev = new_log_ev_1 + new_log_ev_2 - log_ev_1 - log_ev_2;

    // Check if new partition is accepted using metropolis acceptance probability
    double p = exp(diff_log_ev / settings.temp);
    double u = ((double) rand() / (RAND_MAX));

    if (p > u){
        // Accept the new partition
        partition[comp_1_index] = new_comp_1;
        partition[comp_2_index] = new_comp_2;

        if (bit_count(new_comp_1) == 1){
            settings.occupied_comp2 -= (ONE << comp_1_index);
        }
        if (bit_count(comp_2) == 1){
            settings.occupied_comp2 += (ONE << comp_2_index);
        }
        return 0;
    }
    return 1;
}