#include "search/search.h"
#include "search/exhaustive.h"

MCM MCMSearch::exhaustive_search(Data& data) {
    // Clear from previous search
    this->log_evidence_trajectory.clear();
    // Initialize an mcm object to store the result
    int n = data.n;
    this->mcm_out = MCM(n);
    this->data = &data;

    // Indicate that this is an exhaustive search
    // Necessary because different data structure is used to store the evidence of ICCs
    this->exhaustive = true;
    // Reserve memory for storage of evidence of icc (2^n - 1 iccs) -> store in a vector because will encounter all of them in an exhaustive search
    __uint128_t n_iccs = 1 << n;
    this->evidence_storage_es.assign(n_iccs-1, 0);

    // Variable to store the log evidence
    double log_evidence;
    // Representation of the partition that can used by the function to calculate the evidence
    std::vector<__uint128_t> partition(n, 0);
    // Initialize arrays to keep track of the next partition to generate
    int a[n];
    int b[n];
    for (int i = 0; i < n; i++){
        a[i] = 0;
        b[i] = 1;
    }
    // Index of first value that is different (from right to left) between a and b
    int j = n - 1;

    while (j != 0){
        // Partition is written as a restricted growth string -> convert it (updates 'partition')
        convert_partition(a, partition, n);
        log_evidence = this->get_log_ev(partition);

        // Check if this is the new best log evidence
        if (log_evidence > this->mcm_out.log_ev){
            // Update the current best
            this->mcm_out.log_ev = log_evidence;
            // Make a hard copy of current partition to store
            this->mcm_out.partition = partition;
        }

        this->log_evidence_trajectory.push_back(log_evidence);

        j = generate_next_partition(a, b, n);
        if (j == 0){
            // All possible partitions are generated
            break;
        }
        // Reset the converted partition
        std::fill(partition.begin(), partition.end(), 0);
    }
    // Calculate the log ev per icc
    this->mcm_out.log_ev_per_icc.assign(n, 0);

    this->mcm_out.n_comp = 0;
    for (int i = 0; i < n; i++){
        if (this->mcm_out.partition[i]){
            this->mcm_out.log_ev_per_icc[i] = this->get_log_ev_icc(this->mcm_out.partition[i]);
            this->mcm_out.n_comp++;
        }
    }
    // Indicate that the search has been done
    this->mcm_out.optimized = true;
    
    return this->mcm_out;
}

int generate_next_partition(int* a, int* b, int n){
    // Compare the last bit
    if (a[n-1] != b[n-1]){
        // Increase the last bit of 'a' by 1 to generate new partition
        a[n-1] += 1;
        return 1;
    }
    // Find the first bit that is different (starting from the right)
    int j = find_j(a, b, n);
    if (j == 0){
        // All bits are the same -> all possible partitions are generated
        return 0;
    }
    // Increase the first bit from the right in 'a' that is different from 'b' by 1
    a[j] += 1;
    if (a[j] == b[j]){
        // Change the number of partitions the next member is allowed to be in
        b[j+1] = b[j] + 1;
    }
    // Set values for all member to the right
    int i = j+1;
    while (i < n){
        // Move variable back to the first component
        a[i] = 0;
        // Change the number of components this member is allowed to be in
        b[i] = b[j+1];
        ++i;
    }
    return 1;
}

int find_j(int* a, int* b, int n){
    int j = n-2;
    while(a[j] == b[j]){--j;}
    return j;
}