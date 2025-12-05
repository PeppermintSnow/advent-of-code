#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 128

/*
 * Finds the largest combination on a given bank/line of numbers
 *
 * @param bank String of numbers
 */
int findLargestCombination(char* bank);

int main(int argc, char** argv) {
    if (argc < 2) { 
        printf("Please specify an input file");
        exit(1);
    }

    FILE* fptr = fopen(argv[1], "r");
    
    if (!fptr) {
        printf("Failed to open file");
        exit(1);
    }

    int password = 0;
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, fptr)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        password += findLargestCombination(buffer);
    }

    printf("The password for part 1 is %d!", password);

    return 0;
}

int findLargestCombination(char* bank) {
    int bankLen = strlen(bank);

    char highestDigit[] = "0";
    char secondHighestDigit[] = "0";

    // Find highest digit
    for (int i = 0; i < bankLen; i++) {
        if ((highestDigit[0] - '0') < (bank[i] - '0')) {
            highestDigit[0] = bank[i];
        }
    }

    int indexOfHighestDigit = strcspn(bank, highestDigit);

    if (bankLen - 1 == indexOfHighestDigit) {
        // If highest digit is at the end
        // Reverse search and swap positions
        secondHighestDigit[0] = highestDigit[0];
        highestDigit[0] = '0';
        for (int i = bankLen - 2; i >= 0; i--) {
            if ((highestDigit[0] - '0') < (bank[i] - '0')) {
                highestDigit[0] = bank[i];
            }
        }
    } else {
        char newBankLen = bankLen - indexOfHighestDigit;
        char newBank[newBankLen];
        strncpy(newBank, bank + indexOfHighestDigit, newBankLen);
        newBank[newBankLen] = '\0';

        // Find second highest digit
        for (int i = 1; i < newBankLen; i++) {
            if ((secondHighestDigit[0] - '0') < (newBank[i] - '0')) {
                secondHighestDigit[0] = newBank[i];
            }
        }
    }

    char highestCombination[3];
    highestCombination[0] = highestDigit[0];
    highestCombination[1] = secondHighestDigit[0];

    return strtol(highestCombination, NULL, 10);
}
