#include "search/search.h"

MCM MCMSearch::greedy_search(Data& data, MCM* init_mcm){
    // Assign variables
    int n = data.n;
    this->data = &data;
    // Initialize an mcm to store the result
    if(!init_mcm){
        // Default initial mcm is the independent one
        this->mcm_in = MCM(n, "independent");
        this->mcm_out = mcm_in;
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

    // Calculate the log ev
    this->mcm_out.log_ev_per_icc.assign(n, 0);
    for (int i = 0; i < this->mcm_out.n_comp; i++){
        this->mcm_out.log_ev_per_icc[i] = this->get_log_ev_icc(this->mcm_out.partition[i]);
    }
    this->mcm_out.log_ev = this->get_log_ev(this->mcm_out.partition);

    // Store log_ev of starting point
    this->log_evidence_trajectory.push_back(this->mcm_out.log_ev);

    // Hierarchical merging procedure
    this->hierarchical_merging();

    place_empty_entries_last(this->mcm_out.partition);
    place_empty_entries_last(this->mcm_out.log_ev_per_icc);
    // Indicate that the search has been done
    this->mcm_out.optimized = true;

    return this->mcm_out;
}

void MCMSearch::hierarchical_merging(){
    int n = this->mcm_out.n;
    // Variables to store the calculated evidences
    double evidence_i;
    double evidence_j;
    double evidence_ij;
    double evidence_diff;

    // Variables to store the best values
    int best_i;
    int best_j;
    double best_evidence;
    double best_evidence_diff;

    while (true){
        best_evidence_diff = 0;
        for (int i = 0; i < n; i++){
            // Skip empty components
            if (this->mcm_out.partition[i] == 0){continue;}
            evidence_i = this->mcm_out.log_ev_per_icc[i];
            for (int j = i+1; j < n; j++){
                // Skip empty components
                if (this->mcm_out.partition[j] == 0){continue;}
                evidence_j = this->mcm_out.log_ev_per_icc[j];
                // Calculate difference in evidence between merged and separate partitions
                evidence_ij = this->get_log_ev_icc(this->mcm_out.partition[i] + this->mcm_out.partition[j]);
                evidence_diff = evidence_ij - evidence_i - evidence_j;
                // Check if the difference is the best merge so far
                if (evidence_diff > best_evidence_diff){
                    // Update the largest increase in evidence
                    best_evidence_diff = evidence_diff;
                    best_evidence = evidence_ij;
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
            this->mcm_out.partition[best_i] += this->mcm_out.partition[best_j];
            this->mcm_out.partition[best_j] = 0;

            this->mcm_out.log_ev_per_icc[best_i] = best_evidence;
            this->mcm_out.log_ev_per_icc[best_j] = 0;

            this->mcm_out.log_ev += best_evidence_diff;
            this->log_evidence_trajectory.push_back(this->mcm_out.log_ev);

            this->mcm_out.n_comp--;
        }
    }
}