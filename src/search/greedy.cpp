#include "search/search.h"

Model MCMSearch::greedy_search(Data& data, Model* init_model){
    // Assign variables
    int n = data.n;
    this->data = &data;
    // Initialize a model to store the result
    if(!init_model){
        // Default initial model is the independent one
        this->model_in = Model(n, "independent");
        this->model_out = Model(n, "independent");
    }
    else{
        // Check if the number of variables in the data and the model match
        if (n != init_model->n) {
            throw std::invalid_argument("Number of variables in the data doesn't match the number of variables in the given model.");
        }
        this->model_in = *init_model;
        this->model_out = *init_model;
    }

    // Clear from previous search
    this->log_evidence_trajectory.clear();
    this->exhaustive = false;

    // Variables to store the calculated evidences
    double evidence_i;
    double evidence_j;
    double evidence_diff;

    // Variables to store the best values
    int best_i;
    int best_j;
    double best_evidence_diff;

    // Store log_ev of starting point
    this->log_evidence_trajectory.push_back(this->get_log_ev(this->model_out.partition));

    while (true){
        best_evidence_diff = 0;
        for (int i = 0; i < n; i++){
            // Skip empty components
            if (this->model_out.partition[i] == 0){continue;}
            evidence_i = this->get_log_ev_icc(this->model_out.partition[i]);
            for (int j = i+1; j < n; j++){
                // Skip empty components
                if (this->model_out.partition[j] == 0){continue;}
                evidence_j = this->get_log_ev_icc(this->model_out.partition[j]);
                // Calculate difference in evidence between merged and separate partitions
                evidence_diff = this->get_log_ev_icc(this->model_out.partition[i] + this->model_out.partition[j]) - evidence_i - evidence_j;
                // Check if the difference is the best merge so far
                if (evidence_diff > best_evidence_diff){
                    // Update the largest increase in evidence
                    best_evidence_diff = evidence_diff;
                    // Keep track of the components that resulted in the largest increase
                    best_i = i;
                    best_j = j;
                }
            }
        }
        // Stop the algorithm if the evidence did not increase
        if (best_evidence_diff == 0){
            break;
        }
        else{
            //std::cout << "Merging components " << best_i << " and " << best_j << " Evidence difference: "<<  best_evidence_diff << std::endl;
            // Merge the two components that results in the biggest increase in evidence
            this->model_out.partition[best_i] += this->model_out.partition[best_j];
            this->model_out.partition[best_j] = 0;
            this->log_evidence_trajectory.push_back(this->get_log_ev(this->model_out.partition));
        }
    }
    // Store the best log evidence found using the greedy merging scheme
    this->model_out.log_ev = this->get_log_ev(this->model_out.partition);
    // Calculate the log ev per icc
    this->model_out.n_comp = 0;
    this->model_out.log_ev_per_icc.assign(n, 0);
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