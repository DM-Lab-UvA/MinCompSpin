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