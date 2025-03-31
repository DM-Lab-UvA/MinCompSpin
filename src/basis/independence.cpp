#include "basis/independence.h"

int gcd(int a, int b){
    if (a == 0){
        return b;
    }
    return gcd(b % a, a);
}

bool is_linearly_independent(std::vector<std::vector<uint8_t>>& matrix, int n, int q){
    // Row reduce the matrix to get the indices of the independent columns
    std::vector<uint8_t> column_indices = row_reduce(matrix, q);
    // Independent of there are n of these columns
    return (column_indices.size() == n);
}

std::vector<uint8_t> row_reduce(std::vector<std::vector<uint8_t>> matrix, int q){
    int n_rows = matrix.size();
    int n_columns = matrix[0].size();

    // Find the first n independent columns using Gaussian elimination mod q of the matrix
    std::vector<uint8_t> column_indices;
    
    int pivot_value, value, factor, power;
    int row, col = 0;
    std::vector<uint8_t> tmp_row;

    // Loop over the rows
    for (int i = 0; i < n_rows; ++i){
        row = i;
        // Find the first non-zero element
        while (true){
            if (gcd(matrix[row][col], q) != 1){
                row++;
                if (row == n_rows){
                    // No non-zero element in this column --> linearly dependent on previous columns
                    // Check the next column starting from the ith row again
                    row = i;
                    col++;
                }
                if (col == n_columns){
                    // All columns have been checked
                    return column_indices;
                }
            }
            else{
                // Independent column found
                pivot_value = matrix[row][col];
                column_indices.push_back(col);
                break;
            }
        }
        // Swap rows if non-zero element was not the first element
        if (row != i){
            tmp_row = matrix[row];
            matrix[row] = matrix[i];
            matrix[i] = tmp_row;
            row = i;
        }

        // Put all element in the rows below to zero by adding multiples of the current row
        for (int j = row+1; j < n_rows; ++j){
            // Check how many times the pivot value must be added to get a zero
            value = matrix[j][col];
            factor = 0;
            while (value % q){
                value += pivot_value;
                ++factor;
            }
            if (factor){
                // Loop over columns in the current row
                for (int k = col; k < n_columns; ++k){
                    matrix[j][k] = (matrix[j][k] + factor * matrix[row][k]) % q;
                }
            }
        }

        // Go to the next column
        ++col;
        // Break if all columns have been searched
        if (col == n_columns){break;}
    }
    return column_indices;
}