#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 128

/*
 * Finds the largest combination on a given bank/line of numbers
 *
 * @param bank String of numbers
 */
void findLargestCombination(char *bank, long int *password);

int main(int argc, char **argv) {
    if (argc < 2) { 
        printf("Please specify an input file");
        exit(1);
    }

    FILE *fptr = fopen(argv[1], "r");
    
    if (!fptr) {
        printf("Failed to open file");
        exit(1);
    }

    long int password[] = {0, 0};

    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, fptr)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        findLargestCombination(buffer, password);
    }

    printf("The password for part 1 is %ld!\n", password[0]);
    printf("The password for part 2 is %ld!", password[1]);

    return 0;
}

void findLargestCombination(char *bank, long int *password) {
    int bankLen = strlen(bank);

    char highestDigitsPart1[3];
    char highestDigitsPart2[13];

    int indicesPart1[2];
    int indicesPart2[12];

    highestDigitsPart1[2] = '\0';
    highestDigitsPart2[12] = '\0';

    char prevIdx = -1;

    // Part 1 (2 digits)
    for (int i = 0; i < 2; i++) {
        int startIdx = prevIdx + 1;
        int endIdx = bankLen - (2 - i);

        highestDigitsPart1[i] = bank[startIdx];
        indicesPart1[i] = startIdx;

        for (int j = startIdx; j <= endIdx; j++) {
            if (highestDigitsPart1[i] < bank[j]) {
                highestDigitsPart1[i] = bank[j];
                indicesPart1[i] = j;
            }
        }

        prevIdx = indicesPart1[i];
    }

    // Part 2 (12 digits)
    prevIdx = -1;
    for (int i = 0; i < 12; i++) {
        int startIdx = prevIdx + 1;
        int endIdx = bankLen - (12 - i);

        highestDigitsPart2[i] = bank[startIdx];
        indicesPart2[i] = startIdx;

        for (int j = startIdx; j <= endIdx; j++) {
            if (highestDigitsPart2[i] < bank[j]) {
                highestDigitsPart2[i] = bank[j];
                indicesPart2[i] = j;
            }
        }

        prevIdx = indicesPart2[i];
    }

    password[0] += strtol(highestDigitsPart1, NULL, 10);
    password[1] += strtol(highestDigitsPart2, NULL, 10);
}
