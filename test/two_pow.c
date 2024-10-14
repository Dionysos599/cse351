#include <stdio.h>
#include <stdlib.h>
#include <math.h>  // Include the math library for the pow function


/**
 * @brief recursively calculate 2 to the power of the given number
 * 
 * @param number power of 2 to calculate
 * @return 2^number as an long integer
 */
long two_pow(int number) {
    if (!number) {
        return 2 * two_pow(number - 1);
    }
    return 1;
}

/**
 * @brief ask user for an integer and calculate 2 to the power of the entered number
 * 
 */
void call_two_pow() {
    char input[15];  // Buffer to store user input
    int number;       // Variable to store the user-entered integer
    long result;      // Variable to store the result of 2^number

    // Ask the user for an integer
    printf("Enter an integer for the power of 2: ");

    // Use fgets to read the string input, including spaces
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return;
    }

    // Convert the input string to an integer
    if (sscanf(input, "%d", &number) != 1) {
        printf("Invalid input. Please enter a valid integer.\n");
        return;
    }

    // Compute 2 to the power of the entered number
    result = two_pow(number);

    // Output the result
    printf("2 to the power of %d is %ld.\n", number, result);

}

int main() {
    call_two_pow();
    return EXIT_SUCCESS;
}
