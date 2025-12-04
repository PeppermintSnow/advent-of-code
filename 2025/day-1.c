#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The dial initially points at 50
#define DIAL_START 50
// Only read 5 bytes per instruction + 1 for null terminator
#define INSTRUCTION_LENGTH 6 

typedef struct Node {
    char* instruction;
    struct Node* next;
} Node;

/*
 * Rotates the dial by the given value
 *
 * @param dial Value which the dial points to
 * @param value Rotation value which follows the form L/R0-99 e.g., L5, R99
 * @return New dial value
 */
int rotateDial(int dial, char* instruction);

int main(int argc, char** argv) {
    int dial = DIAL_START;
    char** instructions;
    FILE* fptr;
    Node* head = malloc(sizeof(Node));

    if (argv[1]) {
        fptr = fopen(argv[1], "r");
    } else {
        printf("Please specify a .csv file");
    }

    if (fptr == NULL) {
        printf("Could not open file");
        exit(1);
    }

    Node* currNode = head;
    char buffer[INSTRUCTION_LENGTH];
    while (fgets(buffer, INSTRUCTION_LENGTH, fptr)) {
        buffer[strcspn(buffer, "\n")] = '\0';

        currNode->instruction = malloc(INSTRUCTION_LENGTH);
        strncpy(currNode->instruction, buffer, INSTRUCTION_LENGTH);

        Node* tempNode = malloc(sizeof(Node));
        tempNode->instruction = NULL;
        tempNode->next = NULL;

        currNode->next = tempNode;
        currNode = currNode->next;
    }

    // Password is the number of times the dial hits 0
    int zeroDials = 0;
    currNode = head;
    while(currNode->next != NULL) {
        dial = rotateDial(dial, currNode->instruction);
        if (dial == 0) zeroDials++;
        currNode = currNode->next;
    }

    currNode = head;
    while(currNode->next != NULL) {
        Node* temp = currNode;
        currNode = currNode->next;
        free(temp->instruction);
        free(temp);
    }

    printf("The password is %d!", zeroDials);
    return 0;
}

int rotateDial(int dial, char* instruction) {
    char direction = instruction[0];
    if (direction != 'L' && direction != 'R') return -1;

    char buffer[INSTRUCTION_LENGTH];
    strncpy(buffer, instruction + 1, INSTRUCTION_LENGTH);
    int rotateValue = strtol(buffer, NULL, 10);

    // Right clockwise, left counter-clockwise
    dial = direction == 'L' ? dial - rotateValue : dial + rotateValue;

    // Dial can only be 0-99
    while (direction == 'L' && dial < 0) {
        dial += 100;
    }

    while (direction == 'R' && dial > 99) {
        dial -= 100;
    }

    return dial;
}
