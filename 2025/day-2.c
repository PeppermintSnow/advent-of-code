#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct LinkedListNode {
    long int first;
    long int last;
    struct LinkedListNode* next;
} LinkedListNode;

/*
 * Converts the entire csv file into a linked list containing
 * the first and last IDs per range
 *
 * @param input The contents of the file
 * @param head Head of the linked list the ranges will be saved to
 * @param tail Tail of the linked list the ranges will be saved to
 */
void processInput(char* input, LinkedListNode** head, LinkedListNode** tail);

/*
 * Finds the invalid ID by specifications of AOC day 2 - part 1
 *
 * @param start First ID in the range
 * @param end Last ID in the range
 * @return Sum of all invalid IDs in the range
 */
long int findInvalidId(long int start, long int end);

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Please specify a file containing the inputs");
        exit(1);
    }

    FILE* fptr = fopen(argv[1], "r");

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
    LinkedListNode* head = NULL;
    LinkedListNode* tail = NULL;
    processInput(buffer, &head, &tail);

    // Find invalid IDs!
    long int password = 0;
    tail = head;
    while (tail != NULL) {
        password += findInvalidId(tail->first, tail->last);
        tail = tail->next;
    }

    printf("The password is %ld!", password);

    // Free LinkedList
    tail = head;
    while (tail != NULL) {
        LinkedListNode* tempNode = tail;
        tail = tail->next;
        free(tempNode);
    }

}

void processInput(char* input, LinkedListNode** head, LinkedListNode** tail) {
    // Range values (delimited by comma)
    char* rangeSavePtr = NULL;
    char* idSavePtr = NULL;
    char* rangeToken = strtok_r(input, ",", &rangeSavePtr);
    while (rangeToken != NULL) {
        LinkedListNode* node = malloc(sizeof(LinkedListNode));
        node->next = NULL;

        // First/Last ID values (delimited by dash)
        char* idToken = strtok_r(rangeToken, "-", &idSavePtr);
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

long int findInvalidId(long int start, long int end) {
    long int sumOfInvalids = 0;
    for (long int i = start; i <= end; i++) {
        // Count digits
        int digits = 0;
        long int temp = i;
        while (temp != 0) {
            temp /= 10;
            digits++;
        }

        // Odd digits can't be invalid
        if (digits % 2 == 1) continue;

        char buffer[digits + 1];
        snprintf(buffer, digits + 1, "%ld", i);
        buffer[digits] = '\0';

        int digitsHalved = digits / 2;
        char firstHalf[digitsHalved + 1];
        char secondHalf[digitsHalved + 1];
        strncpy(firstHalf, buffer, digitsHalved);
        strncpy(secondHalf, buffer + digitsHalved, digitsHalved);
        firstHalf[digitsHalved] = '\0';
        secondHalf[digitsHalved] = '\0';
        
        if (strcmp(firstHalf, secondHalf) == 0) {
            sumOfInvalids += i;
        }
    }
    return sumOfInvalids;
}
