#include <stdio.h>
#include <stdlib.h>

void printMatrix(int n, int (*mat)[n]) {
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++)
            printf("%d ", mat[r][c]);
        printf("\n");
    }
}

void fillMatrix(int n, int (*mat)[n]) {
    printf("\nRandom Matrix Generated:\n");
    for (int r = 0; r < n; r++)
        for (int c = 0; c < n; c++)
            mat[r][c] = rand() % 256;

    printMatrix(n, mat);
}

void swapValues(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void reverseRows(int n, int (*mat)[n]) {
    for (int r = 0; r < n; r++) {
        int *left = mat[r];
        int *right = mat[r] + n - 1;
        while (left < right) {
            swapValues(left, right);
            left++;
            right--;
        }
    }
}

// Transpose the matrix
void rotateMatrix(int n, int (*mat)[n]) {
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            swapValues(&mat[i][j], &mat[j][i]);

    // Reversing each row
    reverseRows(n, mat);

    printf("\nMatrix After 90° Rotation:\n");
    printMatrix(n, mat);
}

void applyAvgFilter(int n, int (*mat)[n]) {
    int previous[n];
    int current[n];

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int total = 0, count = 0;

            for (int x = i - 1; x <= i + 1; x++) {
                for (int y = j - 1; y <= j + 1; y++) {
                    if (x >= 0 && x < n && y >= 0 && y < n) {
                        total += mat[x][y];
                        count++;
                    }
                }
            }
            current[j] = total / count;
        }

        if (i > 0)
            for (int j = 0; j < n; j++)
                mat[i - 1][j] = previous[j];

        for (int j = 0; j < n; j++)
            previous[j] = current[j];
    }

    for (int j = 0; j < n; j++)
        mat[n - 1][j] = previous[j];
    printf("\nMatrix After 3x3 Average Filter:\n");
    printMatrix(n, mat);
}

int main() {
    int matrixSize;
    char againInput;

    do {
        printf("\nEnter matrix size between (2-10): ");
        scanf("%d", &matrixSize);

        if (matrixSize < 2 || matrixSize > 10) {
            printf("Invalid size! Try again.\n");
            continue;
        }

        int mat[matrixSize][matrixSize];

        fillMatrix(matrixSize, mat);
        rotateMatrix(matrixSize, mat);
        applyAvgFilter(matrixSize, mat);

        printf("\nDo you want to generate another matrix? (y/n): ");
        scanf(" %c", &againInput);

    } while (againInput != 'n');

    return 0;
}
