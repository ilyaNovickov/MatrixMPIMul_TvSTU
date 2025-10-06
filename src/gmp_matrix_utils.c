#include "gmp_matrix_utils.h"
#include <stdlib.h>

#define INDEX(matrix, row, colm) ((row) * (matrix)->colms + (colm))

mpz_Matrix mpz_createMatrix(int rows, int colms)
{
    mpz_Matrix mat;
    mat.rows = rows;
    mat.colms = colms;
    mat.data = malloc(sizeof(mpz_t) * rows * colms);
    
    for (int i = 0; i < rows * colms; ++i) {
        mpz_init(mat.data[i]); // инициализация каждого элемента
    }

    return mat;
}

mpz_Matrix mpz_createMatrixForMul(mpz_Matrix* a, mpz_Matrix* b)
{
    mpz_Matrix c;
    if (a->colms != b->rows) {
        c.rows = 0;
        c.colms = 0;
        c.data = NULL;
        return c;
    }
    return mpz_createMatrix(a->rows, b->colms);
}


void mpz_freeMatrix(mpz_Matrix* matrix)
{
    int total = matrix->rows * matrix->colms;
    for (int i = 0; i < total; ++i) {
        mpz_clear(matrix->data[i]);
    }
    free(matrix->data);
    matrix->data = NULL;
    matrix->rows = 0;
    matrix->colms = 0;
}

void mpz_setMatrixAt(mpz_Matrix* matrix, int row, int colm, const mpz_t value)
{
    int idx = INDEX(matrix, row, colm);
    mpz_set(matrix->data[idx], value);
}

void mpz_getMatrixAt(mpz_Matrix* matrix, int row, int colm, mpz_t dest)
{
    int idx = INDEX(matrix, row, colm);
    mpz_set(dest, matrix->data[idx]);
}

inline int mpz_canMatrixAdd(mpz_Matrix* a, mpz_Matrix* b)
{
    return (a->rows == b->rows && a->colms == b->colms);
}

int mpz_addMatrix(mpz_Matrix* c, mpz_Matrix* a, mpz_Matrix* b)
{
    if (!mpz_canMatrixAdd(a, b)) 
        return -1;

    for (int i = 0; i < a->rows; ++i) {
        for (int j = 0; j < a->colms; ++j) {
            int idx = INDEX(a, i, j);
            mpz_add(c->data[idx], a->data[idx], b->data[idx]);
        }
    }

    return 0;
}

inline int mpz_canMatrixMul(mpz_Matrix* a, mpz_Matrix* b)
{
    return (a->colms == b->rows);
}

int mpz_mulMatrix(mpz_Matrix* c, mpz_Matrix* a, mpz_Matrix* b)
{
    if (!mpz_canMatrixMul(a, b)) return -1;

    for (int i = 0; i < a->rows; ++i) {
        for (int j = 0; j < b->colms; ++j) {
            mpz_t sum;
            mpz_init(sum);
            for (int k = 0; k < a->colms; ++k) {
                mpz_t temp;
                mpz_init(temp);
                mpz_mul(temp, a->data[INDEX(a, i, k)], b->data[INDEX(b, k, j)]);
                mpz_add(sum, sum, temp);
                mpz_clear(temp);
            }
            mpz_set(c->data[INDEX(c, i, j)], sum);
            mpz_clear(sum);
        }
    }

    return 0;
}

int mpz_mulAndAddMatrix(mpz_Matrix* c, mpz_Matrix* a, mpz_Matrix* b)
{
    if (!mpz_canMatrixMul(a, b)) return -1;

    for (int i = 0; i < a->rows; ++i) {
        for (int j = 0; j < b->colms; ++j) {
            for (int k = 0; k < a->colms; ++k) {
                mpz_t temp;
                mpz_init(temp);
                mpz_mul(temp, a->data[INDEX(a, i, k)], b->data[INDEX(b, k, j)]);
                mpz_add(c->data[INDEX(c, i, j)], c->data[INDEX(c, i, j)], temp);
                mpz_clear(temp);
            }
        }
    }

    return 0;
}
