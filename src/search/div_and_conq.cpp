#include "search/search.h"
#include "search/div_and_conq.h"
#include "utilities/miscellaneous.h"

Model MCMSearch::divide_and_conquer(Data& data, Model* init_model){
    int n = data.n;
    this->data = &data;
    // Initialize a model to store the result
    if(!init_model){
        // Default initial model is the complete one
        this->model_in = Model(n, "complete");
        this->model_out = Model(n, "complete");
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

    // Store log_ev of starting point
    this->log_evidence_trajectory.push_back(this->get_log_ev(this->model_out.partition));

    // Start recursive algorithm by moving variables from component 0 to component 1
    division(0, 1);

    // Calculate the log evidence of the best MCM found by the search algorithm
    this->model_out.log_ev = this->get_log_ev(this->model_out.partition);
    // Calculate the log ev per icc
    this->model_out.n_comp = 0;
    this->model_out.log_ev_per_icc.assign(n, 0);
    for (int i = 0; i < n; i++){
        if (this->model_out.partition[i]){
            this->model_out.n_comp++;
            this->model_out.log_ev_per_icc[i] = this->get_log_ev_icc(this->model_out.partition[i]);
        }
    }
    place_empty_component_last(this->model_out.partition);
    // Indicate that the search has been done
    this->model_out.optimized = true;

    return this->model_out;
}

int MCMSearch::division(int move_from, int move_to){
    // Number of member in the component that we want to split
    int n_members_1 = bit_count(this->model_out.partition[move_from]);
    // If the component contains 1 variable, no further splits are possible
    if (n_members_1 == 1){return move_to;}

    // Hard copy of the starting partition
    std::vector<__uint128_t> partition = this->model_out.partition;

    // Variables for the difference in evidence before and after split
    double best_evidence_diff = 0;
    double best_evidence_diff_tmp;
    double evidence_diff;

    // Variables to represent the unsplit and split components
    __uint128_t component_1;
    __uint128_t component_2;

    // Variable to indicate which member is moving
    __uint128_t member;

    // Calculate the evidence of the component before splitting (reference point for the difference in evidence)
    double evidence_unsplit_component = this->get_log_ev_icc(partition[move_from]);

    // If the component has more than 2 members, we can skip the last step because it is the same as the first step
    if (n_members_1 > 2){n_members_1 -= 1;}
    
    while (n_members_1 > 1){
        // Initial values
        best_evidence_diff_tmp = -DBL_MAX; 
        component_1 = partition[move_from];
        component_2 = partition[move_to];

        // Move each variable sequentially to component 'move_to'
        for (int i = 0; i <= n_members_1; i++){
            // Integer representation of the bitstring with only a 1 in the position of the (i+1)th bit set to 1 in component
            member = find_member_i(component_1, i+1);
            component_1 -= member;
            component_2 += member;

            // Calculate difference in evidence from splitting
            evidence_diff = this->get_log_ev_icc(component_1) + this->get_log_ev_icc(component_2) - evidence_unsplit_component;

            // Check if this difference is the best one so far (even if negative)
            if (evidence_diff > best_evidence_diff_tmp){
                // Update the temporary best difference
                best_evidence_diff_tmp = evidence_diff;
                // Update partition
                partition[move_from] = component_1;
                partition[move_to] = component_2;
            }

            // Move variable back to component 'move_from'
            component_1 += member;
            component_2 -= member;
        }
        // Check if the split results in an overall improvement of the evidence
        if (best_evidence_diff_tmp > best_evidence_diff){
            // Update the best difference
            best_evidence_diff = best_evidence_diff_tmp;
            // Update the best MCM
            this->model_out.partition[move_from] = partition[move_from];
            this->model_out.partition[move_to] = partition[move_to];

            this->log_evidence_trajectory.push_back(this->get_log_ev(this->model_out.partition));
        }
        // Update number of members
        n_members_1 -= 1;
    }
    // Stop if no split increased the evidence -> component 'move_to' will be empty in that case
    if (this->model_out.partition[move_to] == 0){
        return move_to;
    }
    // If there was a succesful split, component 'move_to' is no longer empty -> increase the index of the first empty component
    int first_empty = move_to + 1;
    // Continue with a split of the first subpart
    first_empty = division(move_from, first_empty);
    // Continue with a split of the second subpart
    first_empty = division(move_to, first_empty);

    return first_empty;
}

__uint128_t find_member_i(__uint128_t component, int i){
    // Counter to keep track of the number of bits set to 1
    int counter = 0;
    // Starting value: integer representation of the bitstring with only the least significant bit set to 1
    __uint128_t position = 1;
    // Continue as long as 'component' has bits set to 1
    while(component){
        // Check if the least significant bit is '1'
        if(component & 1){
            // Increase the counter of the number of bits set to 1
            counter += 1;
        }
        // Check if this is the ith bit set to 1
        if (counter == i){
            return position;
        }
        // Bitshift to the right to check the next bit
        component >>= 1;
        // Update the position with a bitshift to the left
        position <<= 1;
    }
    return position;
}

int index_of_member(__uint128_t member){
    // Starting index
    int i=0;
    // Continue as long as the least significant bit is not a 1
    while((member & 1) == false){
        // Bitshift to the right
        member >>= 1;
        // Update the index
        i++;
    }
    return i;
}