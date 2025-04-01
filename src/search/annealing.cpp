#include "search/search.h"
#include "search/annealing.h"

MCM MCMSearch::simulated_annealing(Data& data, MCM* init_mcm, std::string file_name){
    int n = data.n;
    this->data = &data;
    // Initialize an mcm to store the result
    if(!init_mcm){
        // Default initial mcm is the independent model
        this->mcm_in = MCM(n, "independent");
        this->mcm_out = this->mcm_in;
    }
    else{
        // Check if the number of variables in the data and the mcm match
        if (n != init_mcm->n) {
            throw std::invalid_argument("Number of variables in the data doesn't match the number of variables in the given MCM.");
        }
        this->mcm_in = *init_mcm;
        this->mcm_out = *init_mcm;
    }
    // Clear from previous search
    this->log_evidence_trajectory.clear();
    this->exhaustive = false;

    // Create mcm object to store intermediate results
    MCM mcm_tmp = this->mcm_out;

    // Calculate the log ev
    mcm_tmp.log_ev_per_icc.assign(n, 0);
    for (int i = 0; i < mcm_tmp.n_comp; i++){
        mcm_tmp.log_ev_per_icc[i] = this->get_log_ev_icc(mcm_tmp.partition[i]);
    }
    mcm_tmp.log_ev = this->get_log_ev(mcm_tmp.partition);

    // Store log_ev of starting point
    this->log_evidence_trajectory.push_back(mcm_tmp.log_ev);

    // Write the initial partition to the output file
    if (!file_name.empty()){
        this->output_file = std::unique_ptr<std::ofstream>(new std::ofstream(file_name));
        if (! this->output_file->is_open()){
            std::cerr <<"Error: could not open the given output file.";
        }
        *this->output_file << "============================= \n";
        *this->output_file << "Simulated Annealing Procedure \n";
        *this->output_file << "============================= \n\n";

        *this->output_file << "Data statistics: \n";
        *this->output_file << "---------------- \n\n";

        *this->output_file << "Number of variables: " << data.n << "\n";
        *this->output_file << "Number of states per variable: " << data.q << "\n";
        *this->output_file << "Number of datapoints: " << data.N << "\n";
        *this->output_file << "Number of unique datapoint: " << data.N_unique << "\n";
        *this->output_file << "Entropy of the data: " << data.entropy() << " q-its \n\n";

        *this->output_file << "Initial partition: \n";
        *this->output_file << "------------------ \n\n";

        *this->output_file << "Log-evidence: " << mcm_tmp.log_ev << " = " << mcm_tmp.log_ev / (data.N * log(data.q)) << " q-its/datapoint \n\n";

        print_partition_details_to_file(*this->output_file, mcm_tmp, data.N, data.q);
        *this->output_file << "\n";

        *this->output_file << "Start annealing: \n";
        *this->output_file << "---------------- \n\n";
    }

    // Initialize a struct containing the SA settings
    SA_settings settings(this->SA_T0, this->mcm_out.partition);

    int x;
    int accepted;
    double log_ev;
    int steps_since_improve = 0;

    for (int i = 0; i < this->SA_max_iter; i++){
        if (mcm_tmp.n_comp == data.n){
            x = 0;
        }
        else if (mcm_tmp.n_comp == 1){
            x = 1;
        }
        else{
            x = rand()/(RAND_MAX/3);
        }

        if (x == 0){
            accepted = this->merge_partition(mcm_tmp, settings);
        }
        else if (x == 1){
            accepted = this->split_partition(mcm_tmp, settings);
        }
        else{
            accepted = this->switch_partition(mcm_tmp, settings);
        }

        // Update the temperature
        if (i % this->SA_update_schedule == 0){
            settings.temp = this->SA_T0 / (1 + log(1 + i));
        }

        // Update solution if log evidence improved
        if (mcm_tmp.log_ev > this->mcm_out.log_ev && fabs(mcm_tmp.log_ev - this->mcm_out.log_ev) > settings.epsilon){ 
            // Copy the result to the final mcm
            this->mcm_out.log_ev = mcm_tmp.log_ev;
            this->mcm_out.log_ev_per_icc = mcm_tmp.log_ev_per_icc;
            this->mcm_out.partition = mcm_tmp.partition;
            this->mcm_out.n_comp = mcm_tmp.n_comp;
            steps_since_improve = 0;

            // Write to output file
            if (this->output_file){
                *this->output_file << "Iteration " << i << " \t\t Temperature: " << settings.temp << " \t\t Log-evidence (q-its/datapoint): " << this->mcm_out.log_ev / (this->data->N * log(this->data->q)) << "\n";
            }
        }
        else{steps_since_improve++;}
        this->log_evidence_trajectory.push_back(this->mcm_out.log_ev);

        // Check stopping criteria
        if (steps_since_improve > settings.max_no_improve){
            if (this->output_file){
                *this->output_file << "\nMaximum number of iterations without improvement reached \n\n";
            }
            break;}
    }

    // Hierarchical merging procedure
    this->hierarchical_merging();

    place_empty_entries_last(this->mcm_out.partition);
    place_empty_entries_last(this->mcm_out.log_ev_per_icc);
    // Indicate that the search has been done
    this->mcm_out.optimized = true;

    // Write results to the output file
    if (!file_name.empty()){
        double max_log_likelihood = data.calc_log_likelihood(this->mcm_out.partition);

        *this->output_file << "\nFinal partition: \n";
        *this->output_file << "------------------ \n\n";

        *this->output_file << "Log-evidence: " << this->mcm_out.log_ev << " = " << this->mcm_out.log_ev / (data.N * log(data.q)) << " q-its/datapoint \n";
        *this->output_file << "Max-Log-likelihood: " << max_log_likelihood << " = " << max_log_likelihood / (data.N * log(data.q)) << " q-its/datapoint \n\n";

        print_partition_details_to_file(*this->output_file, this->mcm_out, data.N, data.q);
        *this->output_file << "\n";
        this->output_file.reset();
    }

    return this->mcm_out;
}

int MCMSearch::merge_partition(MCM& mcm, SA_settings& settings){
    if (mcm.n_comp <= 1){return 2;}

    __uint128_t ONE = 1;

    // Choose two random components
    int comp_1 = randomBitIndex(settings.occupied_comp);
    int comp_2 = randomBitIndex(settings.occupied_comp - (ONE << comp_1));

    // Calculate the change in evidence when merging
    __uint128_t merged_comp = mcm.partition[comp_1] + mcm.partition[comp_2];
    double merged_log_ev = this->get_log_ev_icc(merged_comp);
    double diff_log_ev = merged_log_ev - mcm.log_ev_per_icc[comp_1] - mcm.log_ev_per_icc[comp_2];

    // Check if new partition is accepted using metropolis acceptance probability
    double p = exp(diff_log_ev / settings.temp);
    double u = ((double) rand() / (RAND_MAX));

    if (p > u){
        // Accept the new partition
        mcm.partition[comp_1] = merged_comp;
        mcm.partition[comp_2] = 0;
        mcm.log_ev_per_icc[comp_1] = merged_log_ev;
        mcm.log_ev_per_icc[comp_2] = 0;
        mcm.log_ev += diff_log_ev;
        
        settings.occupied_comp -= (ONE << comp_2);
        if ((settings.occupied_comp2 & (ONE << comp_1)) == 0){
            settings.occupied_comp2 += (ONE << comp_1);
        }
        if ((settings.occupied_comp2 & (ONE << comp_2)) == (ONE << comp_2)){
            settings.occupied_comp2 -= (ONE << comp_2);
        }
        mcm.n_comp--;

        return 0;
    }
    return 1;
}

int MCMSearch::split_partition(MCM& mcm, SA_settings& settings){
    if (mcm.n_comp == this->data->n){return 2;}
    if (settings.occupied_comp2 == 0){return 2;}

    __uint128_t ONE = 1;

    // Choose random component containing at least two variables
    int comp_index = randomBitIndex(settings.occupied_comp2);
    __uint128_t comp = mcm.partition[comp_index];

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
    double diff_log_ev = log_ev_1 + log_ev_2 - mcm.log_ev_per_icc[comp_index];

    // Check if new partition is accepted using metropolis acceptance probability
    double p = exp(diff_log_ev / settings.temp);
    double u = ((double) rand() / (RAND_MAX));

    if (p > u){
        // Accept the new partition
        mcm.partition[comp_index] = comp_1;
        mcm.log_ev_per_icc[comp_index] = log_ev_1;
        // Find the first empty component
        int comp_2_index = 0;
        while (mcm.partition[comp_2_index]){comp_2_index++;}
        mcm.partition[comp_2_index] = comp_2;
        mcm.log_ev_per_icc[comp_2_index] = log_ev_2;
        mcm.log_ev += diff_log_ev;

        settings.occupied_comp += (ONE << comp_2_index);
        if (bit_count(comp_1) <= 1){
            settings.occupied_comp2 -= (ONE << comp_index);
        }
        if (bit_count(comp_2) > 1){
            settings.occupied_comp2 += (ONE << comp_2_index);
        }
        mcm.n_comp++;

        return 0;
    }
    return 1;
}

int MCMSearch::switch_partition(MCM& mcm, SA_settings& settings){
    if (mcm.n_comp == 1 || mcm.n_comp == this->data->n){return 2;}
    if (settings.occupied_comp2 == 0){return 2;}

    __uint128_t ONE = 1;

    // Select two random partitions, the first with at least two variables
    int comp_1_index = randomBitIndex(settings.occupied_comp2);
    int comp_2_index = randomBitIndex(settings.occupied_comp - (ONE << comp_1_index));
    __uint128_t comp_1 = mcm.partition[comp_1_index];
    __uint128_t comp_2 = mcm.partition[comp_2_index];

    // Select random variable from first component
    int var = randomBitIndex(comp_1);
    // Move the variable from component 1 to component 2
    __uint128_t new_comp_1 = comp_1 - (ONE << var);
    __uint128_t new_comp_2 = comp_2 + (ONE << var);

    // Calculate the difference in evidence when moving the variable
    double log_ev_1 = this->get_log_ev_icc(new_comp_1);
    double log_ev_2 = this->get_log_ev_icc(new_comp_2);
    double diff_log_ev = log_ev_1 + log_ev_2 - mcm.log_ev_per_icc[comp_1_index] - mcm.log_ev_per_icc[comp_2_index];

    // Check if new partition is accepted using metropolis acceptance probability
    double p = exp(diff_log_ev / settings.temp);
    double u = ((double) rand() / (RAND_MAX));

    if (p > u){
        // Accept the new partition
        mcm.partition[comp_1_index] = new_comp_1;
        mcm.partition[comp_2_index] = new_comp_2;
        mcm.log_ev_per_icc[comp_1_index] = log_ev_1;
        mcm.log_ev_per_icc[comp_2_index] = log_ev_2;
        mcm.log_ev += diff_log_ev;

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