#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 256


int countRolls(int neighborsc, char *neighborsv);

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

    char **grid = malloc(2 * sizeof(char *));
    
    char buffer[BUFFER_SIZE];
    int lineIndex = 0;
    while (fgets(buffer, BUFFER_SIZE, fptr)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        size_t len = strlen(buffer);
        
        if (lineIndex != 0) {
            char **tempPtr = realloc(grid, (lineIndex + 1) * sizeof(char *));

            if (!tempPtr) {
                printf("Failed to reallocate memory");
                for (int i = 0; i < lineIndex; i++) free(grid[i]);
                free(grid);
                exit(1);
            } else {
                grid = tempPtr;
            }
        }

        grid[lineIndex] = malloc(len + 1);
        memcpy(grid[lineIndex], buffer, len);
        grid[lineIndex][len] = '\0';

        lineIndex++;
    }

    int accessibleRolls = 0;
    size_t len = strlen(grid[0]);
    for (int i = 0; i < lineIndex; i++) {
        for (int j = 0; j < len; j++) {
            if (grid[i][j] != '@') {
                printf(".");
                continue;
            }
            int flag = 0;
            // General case
            if (i > 0 && j > 0 && i < lineIndex - 1 && j < len - 1) {
                char neighbors[] = {
                    grid[i - 1][j - 1],     grid[i - 1][j],     grid[i - 1][j + 1],
                    grid[i][j - 1],     /*  grid[i][j],     */  grid[i][j + 1],
                    grid[i + 1][j - 1],     grid[i + 1][j],     grid[i + 1][j + 1],
                };

                flag = countRolls(8, neighbors) < 4;
            }

            // Edge cases
            // Top
            if (i == 0 && j > 0 && j < len - 1) {
                char neighbors[] = {
                /*  grid[i - 1][j - 1],     grid[i - 1][j],     grid[i - 1][j + 1], */
                    grid[i][j - 1],     /*  grid[i][j],     */  grid[i][j + 1],
                    grid[i + 1][j - 1],     grid[i + 1][j],     grid[i + 1][j + 1],
                };

                flag = countRolls(5, neighbors) < 4;

            // Right
            } else if (i > 0 && i < lineIndex - 1 && j == len - 1) {
                char neighbors[] = {
                    grid[i - 1][j - 1],     grid[i - 1][j], /*  grid[i - 1][j + 1], */
                    grid[i][j - 1],     /*  grid[i][j],         grid[i][j + 1],     */
                    grid[i + 1][j - 1],     grid[i + 1][j], /*  grid[i + 1][j + 1], */
                };

                flag = countRolls(5, neighbors) < 4;

            // Bottom
            } else if (i == lineIndex - 1 && j > 0 && j < len - 1) {
                char neighbors[] = {
                    grid[i - 1][j - 1],     grid[i - 1][j],     grid[i - 1][j + 1],
                    grid[i][j - 1],     /*  grid[i][j],     */  grid[i][j + 1],
                /*  grid[i + 1][j - 1],     grid[i + 1][j],     grid[i + 1][j + 1], */
                };

                flag = countRolls(5, neighbors) < 4;

            // Left
            } else if (i > 0 && i < lineIndex - 1 && j == 0) {
                char neighbors[] = {
                /*  grid[i - 1][j - 1], */  grid[i - 1][j],     grid[i - 1][j + 1],
                /*  grid[i][j - 1],         grid[i][j],     */  grid[i][j + 1],
                /*  grid[i + 1][j - 1], */  grid[i + 1][j],     grid[i + 1][j + 1],
                };

                flag = countRolls(5, neighbors) < 4;

            // Top-left
            } else if (i == 0 && j == 0) {
                char neighbors[] = {
                /*  grid[i - 1][j - 1],     grid[i - 1][j],     grid[i - 1][j + 1], */
                /*  grid[i][j - 1],         grid[i][j],     */  grid[i][j + 1],
                /*  grid[i + 1][j - 1], */  grid[i + 1][j],     grid[i + 1][j + 1],
                };

                flag = countRolls(3, neighbors) < 4;

            // Top-right
            } else if (i == 0 && j == len - 1) {
                char neighbors[] = {
                /*  grid[i - 1][j - 1],     grid[i - 1][j],     grid[i - 1][j + 1], */
                    grid[i][j - 1],     /*  grid[i][j],         grid[i][j + 1],     */
                    grid[i + 1][j - 1],     grid[i + 1][j], /*  grid[i + 1][j + 1], */
                };

                flag = countRolls(3, neighbors) < 4;

            // Bottom-right
            } else if (i == lineIndex - 1 && j == len - 1) {
                char neighbors[] = {
                    grid[i - 1][j - 1],     grid[i - 1][j], /*  grid[i - 1][j + 1], */
                    grid[i][j - 1],     /*  grid[i][j],         grid[i][j + 1],     */
                /*  grid[i + 1][j - 1],     grid[i + 1][j],     grid[i + 1][j + 1], */
                };

                flag = countRolls(3, neighbors) < 4;
            
            // Bottom-left
            } else if (i == lineIndex - 1 && j == 0) {
                char neighbors[] = {
                /*  grid[i - 1][j - 1], */  grid[i - 1][j],     grid[i - 1][j + 1],
                /*  grid[i][j - 1],         grid[i][j],     */  grid[i][j + 1],
                /*  grid[i + 1][j - 1],     grid[i + 1][j],     grid[i + 1][j + 1], */
                };

                flag = countRolls(3, neighbors) < 4;
            }

            printf("%c", flag ? 'x' : '@');
            accessibleRolls += flag;
        }
        printf("\n");
    }

    printf("\nAccessible rolls: %d", accessibleRolls);

    for (int i = 0; i < lineIndex; i++) free(grid[i]);
    free(grid);
    fclose(fptr);
    return 0;
}

int countRolls(int neighborsc, char *neighborsv) {
    int numRolls = 0;
    for (int i = 0; i < neighborsc; i++) {
        if (neighborsv[i] == '@') numRolls++;
    }
    return numRolls;
}
