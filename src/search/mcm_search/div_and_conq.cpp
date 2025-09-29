#include "search/mcm_search/mcm_search.h"
#include "search/mcm_search/div_and_conq.h"
#include "utilities/miscellaneous.h"

MCM MCMSearch::divide_and_conquer(Data& data, MCM* init_mcm, std::string file_name){
    int n = data.n;
    this->data = &data;
    // Initialize an mcm to store the result
    if(!init_mcm){
        // Default initial mcm is the complete one
        this->mcm_in = MCM(n, "complete");
        this->mcm_out = MCM(n, "complete");
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

    // Write the initial partition to the output file
    if (!file_name.empty()){
        this->output_file = std::unique_ptr<std::ofstream>(new std::ofstream(file_name));
        if (! this->output_file->is_open()){
            std::cerr <<"Error: could not open the given output file.";
        }
        *this->output_file << "====================================== \n";
        *this->output_file << "Greedy Hierarchical Division Procedure \n";
        *this->output_file << "====================================== \n\n";

        *this->output_file << "Data statistics \n";
        *this->output_file << "--------------- \n\n";

        *this->output_file << "Number of variables: " << data.n << "\n";
        *this->output_file << "Number of states per variable: " << data.q << "\n";
        *this->output_file << "Number of datapoints: " << data.N << "\n";
        *this->output_file << "Number of synthetic datapoints: " << data.N_synthetic << "\n";
        *this->output_file << "Number of unique datapoint: " << data.N_unique << "\n";
        *this->output_file << "Entropy of the data: " << data.entropy() << " q-its \n\n";

        *this->output_file << "Initial partition \n";
        *this->output_file << "----------------- \n\n";

        *this->output_file << "Log-evidence: " << this->mcm_out.log_ev << " = " << this->mcm_out.log_ev / (data.N_synthetic * log(data.q)) << " q-its/datapoint \n\n";

        print_partition_details_to_file(*this->output_file, this->mcm_out, data.N_synthetic, data.q);
        *this->output_file << "\n";
    
        *this->output_file << "Start dividing \n";
        *this->output_file << "-------------- \n\n";
    }

    // Start recursive algorithm by moving variables from component 0 to component 1
    division(0, 1);

    place_empty_entries_last(this->mcm_out.partition);
    place_empty_entries_last(this->mcm_out.log_ev_per_icc);
    // Indicate that the search has been done
    this->mcm_out.optimized = true;

    // Write results to the output file
    if (!file_name.empty()){
        double max_log_likelihood = data.calc_log_likelihood(this->mcm_out.partition);

        *this->output_file << "Stop dividing \n";
        *this->output_file << "-------------- \n\n";
        
        *this->output_file << "\nFinal partition \n";
        *this->output_file << "----------------- \n\n";

        *this->output_file << "Log-evidence: " << this->mcm_out.log_ev << " = " << this->mcm_out.log_ev / (data.N_synthetic * log(data.q)) << " q-its/datapoint \n";
        *this->output_file << "Max-Log-likelihood: " << max_log_likelihood << " = " << max_log_likelihood / (data.N_synthetic * log(data.q)) << " q-its/datapoint \n\n";

        print_partition_details_to_file(*this->output_file, this->mcm_out, data.N_synthetic, data.q);
        *this->output_file << "\n";
        this->output_file.reset();
    }

    return this->mcm_out;
}

int MCMSearch::division(int move_from, int move_to){
    // Number of member in the component that we want to split
    int n_members_1 = bit_count(this->mcm_out.partition[move_from]);
    // If the component contains 1 variable, no further splits are possible
    if (n_members_1 == 1){return move_to;}

    // Hard copy of the starting partition
    std::vector<__uint128_t> partition = this->mcm_out.partition;

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
            this->mcm_out.partition[move_from] = partition[move_from];
            this->mcm_out.partition[move_to] = partition[move_to];
        }
        // Update number of members
        n_members_1 -= 1;
    }
    // Stop if no split increased the evidence -> component 'move_to' will be empty in that case
    if (this->mcm_out.partition[move_to] == 0){
        return move_to;
    }
    // Update mcm results
    this->mcm_out.n_comp++;
    this->mcm_out.log_ev_per_icc[move_from] = this->get_log_ev_icc(this->mcm_out.partition[move_from]);
    this->mcm_out.log_ev_per_icc[move_to] = this->get_log_ev_icc(this->mcm_out.partition[move_to]);
    this->mcm_out.log_ev = this->get_log_ev(this->mcm_out.partition);
    
    this->log_evidence_trajectory.push_back(this->mcm_out.log_ev);

    if (this->output_file){
        *this->output_file << "Splitting component " << move_from << "\t Log-evidence (q-its/datapoint): " << this->mcm_out.log_ev / (this->data->N_synthetic * log(this->data->q)) << "\n";
        *this->output_file << "\t Component " << move_from << " : \t" + int_to_string(this->mcm_out.partition[move_from], this->mcm_out.n) << "\n";
        *this->output_file << "\t Component " << move_to << " : \t" + int_to_string(this->mcm_out.partition[move_to], this->mcm_out.n) << "\n\n";
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