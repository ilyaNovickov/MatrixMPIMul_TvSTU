#include "matrix_utils_f.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//Макрос для определения адреса указанного элемента массива
#define INDEXF(mat, row, col) ((row) * (mat)->colms + (col))

MatrixF createMatrixF(int rows, int colms) {
    MatrixF mat;
    mat.rows = rows;
    mat.colms = colms;
    mat.data = (float*)calloc(rows * colms, sizeof(float));
    return mat;
}

MatrixF createMatrixFForMul(MatrixF* a, MatrixF* b) {
    if (a->colms != b->rows) {
        MatrixF empty = {0, 0, NULL};
        return empty;
    }
    return createMatrixF(a->rows, b->colms);
}

void freeMatrixF(MatrixF* matrix) {
    if (matrix->data != NULL) {
        free(matrix->data);
        matrix->data = NULL;
    }
    matrix->rows = 0;
    matrix->colms = 0;
}

void setMatrixFAt(MatrixF* matrix, int row, int colm, float value) {
    matrix->data[INDEXF(matrix, row, colm)] = value;
}

float getMatrixFAt(MatrixF* matrix, int row, int colm) {
    return matrix->data[INDEXF(matrix, row, colm)];
}

int canMatrixFAdd(MatrixF* a, MatrixF* b) {
    return (a->rows == b->rows && a->colms == b->colms);
}

int addMatrixF(MatrixF* c, MatrixF* a, MatrixF* b) {
    if (!canMatrixFAdd(a, b)) return -1;

    for (int i = 0; i < a->rows; ++i) {
        for (int j = 0; j < a->colms; ++j) {
            int idx = INDEXF(a, i, j);
            c->data[idx] = a->data[idx] + b->data[idx];
        }
    }

    return 0;
}

int canMatrixFMul(MatrixF* a, MatrixF* b) {
    return (a->colms == b->rows);
}

int mulMatrixF(MatrixF* c, MatrixF* a, MatrixF* b) {
    if (!canMatrixFMul(a, b)) return -1;

    for (int i = 0; i < a->rows; ++i) {
        for (int j = 0; j < b->colms; ++j) {
            float sum = 0.0f;
            for (int k = 0; k < a->colms; ++k) {
                sum += a->data[INDEXF(a, i, k)] * b->data[INDEXF(b, k, j)];
            }
            c->data[INDEXF(c, i, j)] = sum;
        }
    }

    return 0;
}

int mulAndAddMatrixF(MatrixF* c, MatrixF* a, MatrixF* b) {
    if (!canMatrixFMul(a, b)) return -1;

    for (int i = 0; i < a->rows; ++i) {
        for (int j = 0; j < b->colms; ++j) {
            for (int k = 0; k < a->colms; ++k) {
                c->data[INDEXF(c, i, j)] += a->data[INDEXF(a, i, k)] * b->data[INDEXF(b, k, j)];
            }
        }
    }

    return 0;
}

void fillMatrixFRandom(MatrixF* matrix) {
    for (int i = 0; i < matrix->rows; ++i) {
        for (int j = 0; j < matrix->colms; ++j) {
            // Генерация случайного float в диапазоне [-1.0, 1.0]
            float value = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
            setMatrixFAt(matrix, i, j, value);
        }
    }
}

int writeMatrixFinFile(char* path, MatrixF* matrix)
{
    FILE* file = fopen(path, "w+");

    if (file == NULL)
    {
        return 1;
    }

    for (int row = 0; row < matrix->rows; row++)
    {
        for (int col = 0; col < matrix->colms; col++)
        {
            fprintf(file, "%f;", getMatrixFAt(matrix, row, col));
        }
        fprintf(file, "\n");
    }

    fclose(file);

    return 0;
}