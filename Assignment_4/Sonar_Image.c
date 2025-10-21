#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void printMatrix(int matrixSize, int *mat) {
    for (int r = 0; r < matrixSize; r++) {
        for (int c = 0; c < matrixSize; c++)
            printf("%d ", *(mat + r * matrixSize + c));
        printf("\n");
    }
}

void fillMatrix(int matrixSize, int *mat) {
    printf("\nRandom Matrix Generated:\n");
    for (int r = 0; r < matrixSize; r++) {
        for (int c = 0; c < matrixSize; c++) {
            *(mat + r * matrixSize + c) = rand() % 256;
        }
    }
    printMatrix(matrixSize, mat);
}

void swapValues(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void reverseRows(int matrixSize, int *mat) {
    for (int r = 0; r < matrixSize; r++) {
        int *left = mat + r * matrixSize;
        int *right = mat + r * matrixSize + matrixSize - 1;
        while (left < right) {
            swapValues(left, right);
            left++;
            right--;
        }
    }
}

// Rotate matrix by 90 degree clockwise
void rotateMatrix(int matrixSize, int *mat) {
    for (int i = 0; i < matrixSize; i++) {
        for (int j = i + 1; j < matrixSize; j++) {
            swapValues(mat + i * matrixSize + j, mat + j * matrixSize + i);
        }
    }
  //reverse rows
    reverseRows(matrixSize, mat);

    printf("\nMatrix After 90° Rotation:\n");
    printMatrix(matrixSize, mat);
}

void applyAvgFilter(int matrixSize, int *mat) {
    int *previous = (int *)malloc(matrixSize * sizeof(int));
    int *current = (int *)malloc(matrixSize * sizeof(int));

    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            int total = 0, count = 0;

            for (int x = i - 1; x <= i + 1; x++) {
                for (int y = j - 1; y <= j + 1; y++) {
                    if (x >= 0 && x < matrixSize && y >= 0 && y < matrixSize) {
                        total += *(mat + x * matrixSize + y);
                        count++;
                    }
                }
            }
            *(current + j) = total / count;
        }

        if (i > 0) {
            for (int j = 0; j < matrixSize; j++)
                *(mat + (i - 1) * matrixSize + j) = *(previous + j);
        }

        for (int j = 0; j < matrixSize; j++)
            *(previous + j) = *(current + j);
    }

    for (int j = 0; j < matrixSize; j++)
        *(mat + (matrixSize - 1) * matrixSize + j) = *(previous + j);

    printf("\nMatrix After 3x3 Average Filter:\n");
    printMatrix(matrixSize, mat);

    free(previous);
    free(current);
}

int main() {
    int matrixSize;
    char againInput;

    srand(time(NULL));  

    do {
        printf("\nEnter matrix size between (2-10): ");
        scanf("%d", &matrixSize);

        if (matrixSize < 2 || matrixSize > 10) {
            printf("Invalid size! Try again.\n");
            continue;
        }

        int *mat = (int *)malloc(matrixSize * matrixSize * sizeof(int));

        fillMatrix(matrixSize, mat);
        rotateMatrix(matrixSize, mat);
        applyAvgFilter(matrixSize, mat);

        free(mat);

        printf("\nDo you want to generate another matrix? (y/n): ");
        scanf(" %c", &againInput);

    } while (againInput != 'n');

    return 0;
}
