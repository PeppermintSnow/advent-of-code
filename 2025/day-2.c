#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct LinkedListNode {
    long int first;
    long int last;
    struct LinkedListNode *next;
} LinkedListNode;

/*
 * Converts the entire csv file into a linked list containing
 * the first and last IDs per range
 *
 * @param input The contents of the file
 * @param head Head of the linked list the ranges will be saved to
 * @param tail Tail of the linked list the ranges will be saved to
 */
void processInput(char *input, LinkedListNode **head, LinkedListNode **tail);

/*
 * Finds the invalid ID in a range by specifications of AOC day 2
 *
 * @param start First ID in the range
 * @param end Last ID in the range
 * @return Sum of all invalid IDs in the range
 */
void findInvalidId(long int start, long int end, long int *password);

/*
 * Determines whether the given ID is invalid
 *
 */
long int isInvalidId(long int id);

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Please specify a file containing the inputs");
        exit(1);
    }

    FILE *fptr = fopen(argv[1], "r");

    if (fptr == NULL) {
        printf("Failed to open file");
        exit(1);
    }

    // Read input
    fseek(fptr, 0, SEEK_END);
    long int bufferSize = ftell(fptr);
    char buffer[bufferSize + 1];

    fseek(fptr, 0, SEEK_SET);
    fread(buffer, sizeof(char), bufferSize, fptr);
    buffer[bufferSize] = '\0';

    fclose(fptr);
    
    // Save input as linked list
    LinkedListNode *head = NULL;
    LinkedListNode *tail = NULL;
    processInput(buffer, &head, &tail);

    // Find invalid IDs!
    // Part 1: IDs are invalid if the number is made of a sequence repeated twice
    // Part 2: IDs are invalid if the number is made of a sequence repeated AT LEAST twice
    // [0] Password for part 1; [1] Password for part 2
    long int *password = malloc(sizeof(long int) * 2);
    password[0] = 0;
    password[1] = 0;

    tail = head;
    while (tail != NULL) {
        findInvalidId(tail->first, tail->last, password);
        tail = tail->next;
    }

    printf("The password for part 1 is %ld!\n", password[0]);
    printf("The password for part 2 is %ld!\n", password[1]);

    // Free LinkedList
    tail = head;
    while (tail != NULL) {
        LinkedListNode *tempNode = tail;
        tail = tail->next;
        free(tempNode);
    }

}

void processInput(char *input, LinkedListNode **head, LinkedListNode **tail) {
    // Range values (delimited by comma)
    char *rangeSavePtr = NULL;
    char *idSavePtr = NULL;
    char *rangeToken = strtok_r(input, ",", &rangeSavePtr);
    while (rangeToken != NULL) {
        LinkedListNode *node = malloc(sizeof(LinkedListNode));
        node->next = NULL;

        // First/Last ID values (delimited by dash)
        char *idToken = strtok_r(rangeToken, "-", &idSavePtr);
        int isLastId = 0;
        while (idToken != NULL) {
            if (!isLastId) {
                node->first = strtol(idToken, NULL, 10);
                isLastId = 1;
            } else {
                node->last = strtol(idToken, NULL, 10);
            }

            idToken = strtok_r(NULL, "-", &idSavePtr);
        }

        if (!*head) {
            *head = node;
            *tail = node;
        } else {
            (*tail)->next = node;
            *tail = node;
        }

        rangeToken = strtok_r(NULL, ",", &rangeSavePtr);
    }
}

void findInvalidId(long int start, long int end, long int *password) {
    for (long int i = start; i <= end; i++) {
        // Count digits
        int digits = 0;
        long int temp = i;
        while (temp != 0) {
            temp /= 10;
            digits++;
        }

        char buffer[digits + 1];
        snprintf(buffer, digits + 1, "%ld", i);
        buffer[digits] = '\0';

        int isCountedPart1 = 0;
        int isCountedPart2 = 0;
        // Take factors of digits
        for (int j = 2; j < digits; j++) {
            if (digits % j != 0) continue;

            int segmentCount = digits / j;
            if (segmentCount == 0) continue;

            char **idSegments = malloc(segmentCount * sizeof(char*));

            for (int k = 0; k < segmentCount; k++) {
                idSegments[k] = malloc(j + 1);
                strncpy(idSegments[k], buffer + (k * j), j);
                idSegments[k][j] = '\0';
            }

            int isInvalid = 1;
            for (int k = 0; k < segmentCount; k++) {
                if (strcmp(idSegments[0], idSegments[k]) != 0) {
                    isInvalid = 0;
                    break;
                }
            }

            // Free memory
            for (int k = 0; k < segmentCount; k++) free(idSegments[k]);
            free(idSegments);

            if (!isInvalid) continue;

            if (segmentCount == 2 && !isCountedPart1) {
                password[0] += i;
                isCountedPart1 = 1;
            }

            if (!isCountedPart2) {
                password[1] += i;
                isCountedPart2 = 1;
            }
        }

        // Check for one repeated digits
        if (!isCountedPart2 && digits > 1) {
            int isInvalid = 1;
            for (int j = 0; buffer[j] != '\0'; j++) {
                if (buffer[0] != buffer[j]) {
                    isInvalid = 0;
                    break;
                }
            }

            if (isInvalid) {
                password[1] += i;
                isCountedPart2 = 1;
            }
        }
    }
}
