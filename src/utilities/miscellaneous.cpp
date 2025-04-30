#include "utilities/miscellaneous.h"

int bit_count(__uint128_t integer){
    int size = 0;
    while(integer){
        // Check if last bit is equal to 1
        if (integer & 1){
            size += 1;
        }
        // Bitshift to the right
        integer >>= 1;
    }
    return size;
}

int randomBitIndex(__uint128_t integer){
    __uint128_t ONE = 1;
    std::string r;

	int nb = bit_count(integer);

	int x = 0;
	int y = 0;
	
	int my_index = rand()/(RAND_MAX/nb);
	int loc = 0;

	while(integer) {

		r = ((integer & ONE) + '0');

		if (r == "1") {
			if (x == my_index){
				loc = y;
				break;
			}
			x++;
		}
		integer >>= 1;
		y++;
	}	

	return loc;
}

__uint128_t random_128_int(int n){
    __uint128_t integer=0;
    integer += rand() % 2;
    for (int i = 1; i < n; i++){
        integer <<= 1;
        integer += rand() % 2;
        
    }
    return integer;
}

std::string int_to_string(__uint128_t integer, int n){
    // String representation of component
    std::string comp(n, '0');
    for (int j = 0 ; j < n; ++j){
        if (integer & 1){
            comp[j] = '1';
        }
        integer >>= 1;
    }
    return comp;
}

__uint128_t string_to_int(std::string string, int n){
    __uint128_t integer = 0;
    __uint128_t element = 1;
    for (int i = 0; i < n; i++){
        int value = string[i] - '0';
        if (value){
            integer += element;
        }
        element <<= 1;
    }
    return integer;
}

void convert_string_to_vector(std::vector<__uint128_t>& vec, std::string& str, int n, int q){
    // Set all elements equal to zero
    std::fill(vec.begin(), vec.end(), 0);
    // Variable for the integer value of the ith bit
    __uint128_t element = 1;
    // Loop over the variables
    for (int i = 0; i < n; ++i){
        // Convert value of variable i from string to integer
        int value = str[i] - '0';
        // Check if the value is between 0 and q-1
        if (value > q - 1) {
            throw std::invalid_argument("Entries in the file should only contain values between 0 and q-1.");
        }
        int bit = 0;
        while (value){
            // Check if last bit in the binary representation is nonzero
            if (value & 1){
                vec[bit] += element;
            }
            // Bitshift to the right
            ++bit;
            value >>= 1;
        }
        // Bitshift to the left to get the decimal value of the next variable
        element <<= 1;
    }
}

void convert_8bit_vec_to_128bit_vec(std::vector<__uint128_t>& vec128, const std::vector<uint8_t>& vec8, int n, int q){
    // Set all elements equal to zero
    std::fill(vec128.begin(), vec128.end(), 0);
    // Variable for the integer value of the ith bit
    __uint128_t element = 1;
    // Loop over the variables
    for (int i = 0; i < n; ++i){
        // Convert value of variable i from string to integer
        int value = vec8[i];
        // Check if the value is between 0 and q-1
        if (value > q - 1) {
            throw std::invalid_argument("The vector should only contain values between 0 and q-1.");
        }
        int bit = 0;
        while (value){
            // Check if last bit in the binary representation is nonzero
            if (value & 1){
                vec128[bit] += element;
            }
            // Bitshift to the right
            ++bit;
            value >>= 1;
        }
        // Bitshift to the left to get the decimal value of the next variable
        element <<= 1;
    }
}

void convert_8bit_vec_to_128bit_vec_unsafe(std::vector<__uint128_t>& vec128, const std::vector<uint8_t>& vec8, int n, int q){
    // Set all elements equal to zero
    std::fill(vec128.begin(), vec128.end(), 0);
    // Variable for the integer value of the ith bit
    __uint128_t element = 1;
    // Loop over the variables
    for (int i = 0; i < n; ++i){
        // Convert value of variable i from string to integer
        int value = vec8[i];
        int bit = 0;
        while (value){
            // Check if last bit in the binary representation is nonzero
            if (value & 1){
                vec128[bit] += element;
            }
            // Bitshift to the right
            ++bit;
            value >>= 1;
        }
        // Bitshift to the left to get the decimal value of the next variable
        element <<= 1;
    }
}

std::string convert_128bit_vec_to_string(std::vector<__uint128_t>& vec128, int n){
    // Create string with n zero entries
    std::vector<char> state(n, '0');

    int element;
    // Loop over the variables
    for (int j = 0; j < n; ++j){
        element = 1;
        // Loop over the entries of the vector
        for (int i = 0; i < vec128.size(); ++i){
            if (vec128[i] & 1){
                state[j] += element;
            }
            element <<= 1;
            vec128[i] >>= 1;
        }
    }
    std::string str(state.begin(), state.end());
    return str;
}