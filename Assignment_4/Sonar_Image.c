#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generateRandomMatrix(int *matrix, int size)
{
    srand(time(NULL));
    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            *(matrix + row * size + col) = rand() % 256;
        }
    }
}

void printMatrix(int *matrix, int size, const char *title)
{
    printf("\n%s\n", title);
    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            printf("%3d ", *(matrix + row * size + col));
        }
        printf("\n");
    }
}

void transposeMatrix(int *matrix, int size)
{
    for (int row = 0; row < size; row++)
    {
        for (int col = row + 1; col < size; col++)
        {
            int temp = *(matrix + row * size + col);
            *(matrix + row * size + col) = *(matrix + col * size + row);
            *(matrix + col * size + row) = temp;
        }
    }
}

void reverseRows(int *matrix, int size)
{
    for (int row = 0; row < size; row++)
    {
        int left = 0, right = size - 1;
        while (left < right)
        {
            int temp = *(matrix + row * size + left);
            *(matrix + row * size + left) = *(matrix + row * size + right);
            *(matrix + row * size + right) = temp;
            left++;
            right--;
        }
    }
}

void rotateMatrix90(int *matrix, int size)
{
    transposeMatrix(matrix, size);
    reverseRows(matrix, size);
}

int computeNeighborAverage(int *matrix, int row, int col, int size)
{
    int sum = 0, count = 0;
    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            int neighborRow = row + dx;
            int neighborCol = col + dy;
            if (neighborRow >= 0 && neighborRow < size && neighborCol >= 0 && neighborCol < size)
            {
                sum += *(matrix + neighborRow * size + neighborCol);
                count++;
            }
        }
    }
    return sum / count;
}

void applySmoothingFilter(int *inputMatrix, int *outputMatrix, int size)
{
    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            *(outputMatrix + row * size + col) = computeNeighborAverage(inputMatrix, row, col, size);
        }
    }
}

int getValidatedMatrixSize()
{
    int matrixSize;
    char extraChar;

    while (1)
    {
        printf("Enter matrix size (between 2 and 10): ");
        if (scanf("%d%c", &matrixSize, &extraChar) != 2 || extraChar != '\n')
        {
            printf("Invalid input! Please enter a valid integer.\n");
            while (getchar() != '\n');  
            continue;
        }

        if (matrixSize < 2 || matrixSize > 10)
        {
            printf("Invalid size! Please enter a number between 2 and 10.\n");
            continue;
        }

        return matrixSize;
    }
}

int main()
{
    int matrixSize;
    char choice;

    do
    {
        matrixSize = getValidatedMatrixSize();

        int *originalMatrix = (int *)malloc(matrixSize * matrixSize * sizeof(int));
        int *smoothedMatrix = (int *)malloc(matrixSize * matrixSize * sizeof(int));

        generateRandomMatrix(originalMatrix, matrixSize);
        printMatrix(originalMatrix, matrixSize, "Original Matrix:");

        rotateMatrix90(originalMatrix, matrixSize);
        printMatrix(originalMatrix, matrixSize, "Matrix after 90 degree Rotation:");

        applySmoothingFilter(originalMatrix, smoothedMatrix, matrixSize);
        printMatrix(smoothedMatrix, matrixSize, "Matrix after 3x3 Smoothing Filter:");

        free(originalMatrix);
        free(smoothedMatrix);

        printf("\nDo you want to generate another matrix? (y/n): ");
        scanf(" %c", &choice);

    } while (choice == 'y' || choice == 'Y');

    printf("\nProgram terminated successfully.\n");
    return 0;
}
