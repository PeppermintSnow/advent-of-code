#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The dial initially points at 50
#define DIAL_START 50
// Only read 5 bytes per instruction + 1 for null terminator
#define INSTRUCTION_LENGTH 6

typedef struct Node {
    char *instruction;
    struct Node *next;
} Node;

/*
 * Rotates the dial by the given value
 *
 * @param dial Value which the dial points to
 * @param zeroDials Number of times the dial HITS zero
 * @param value Rotation value which follows the form L/R0-99 e.g., L5, R99
 * @return New dial value
 */
int rotateDial(int dial, int *timesDialHitsZero, char *instruction);

int main(int argc, char **argv) {
    int dial = DIAL_START;
    char **instructions;
    FILE *fptr;

    if (argv[1]) {
        fptr = fopen(argv[1], "r");
    } else {
        printf("Please specify a .csv file");
    }

    if (fptr == NULL) {
        printf("Could not open file");
        exit(1);
    }

    Node *head = NULL;
    Node *tail = NULL;

    char buffer[INSTRUCTION_LENGTH];
    while (fgets(buffer, INSTRUCTION_LENGTH, fptr)) {
        buffer[strcspn(buffer, "\n")] = '\0';

        Node *tempNode = malloc(sizeof(Node));

        tempNode->instruction = malloc(INSTRUCTION_LENGTH);
        strncpy(tempNode->instruction, buffer, INSTRUCTION_LENGTH);
        tempNode->next = NULL;

        if (!head) {
            head = tempNode;
            tail = tempNode;
        } else {
            tail->next = tempNode;
            tail = tempNode;
        }
    }

    // Part 1 states that the number of times the dial STOPS at 0 is the password
    // Part 2 states that the number of times the dial PASSES 0 is the password
    int timesDialHitsZero = 0;
    int timesDialStopsAtZero = 0;
    tail = head;
    while(tail != NULL) {
        dial = rotateDial(dial, &timesDialHitsZero, tail->instruction);
        if (dial == 0) timesDialStopsAtZero++;
        tail = tail->next;
    }

    tail = head;
    while(tail != NULL) {
        Node *temp = tail;
        tail = tail->next;
        free(temp->instruction);
        free(temp);
    }

    printf("The password is for part 1 is %d!\n", timesDialStopsAtZero);
    printf("The password is for part 2 is %d!\n", timesDialHitsZero);
    return 0;
}

int rotateDial(int dial, int *timesDialHitsZero, char *instruction) {
    char direction = instruction[0];
    if (direction != 'L' && direction != 'R') exit(1);

    int rotateValue = strtol(instruction + 1, NULL, 10);

    // Brute force method cuz ts makes me wanna cry
    int step = (direction == 'L') ? -1 : 1;
    for (int i = 0; i < rotateValue; i++) {
        dial += step;
        if (dial < 0) dial = 99;
        if (dial > 99) dial = 0;

        if (dial == 0) (*timesDialHitsZero)++;
    }

    return dial;
}
