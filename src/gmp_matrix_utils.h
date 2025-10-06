#ifndef MATRIX_UTILS_GMP_H
#define MATRIX_UTILS_GMP_H

#include <gmp.h>

typedef struct mpz_Matrix {
    int rows;
    int colms;
    mpz_t* data; // Массив mpz_t (одномерный)
} mpz_Matrix;

mpz_Matrix mpz_createMatrix(int rows, int colms);
mpz_Matrix mpz_createMatrixForMul(mpz_Matrix* a, mpz_Matrix* b);

void mpz_freeMatrix(mpz_Matrix* matrix);

void mpz_setMatrixAt(mpz_Matrix* matrix, int row, int colm, const mpz_t value);
void mpz_getMatrixAt(mpz_Matrix* matrix, int row, int colm, mpz_t dest);

int mpz_canMatrixAdd(mpz_Matrix* a, mpz_Matrix* b);
int mpz_addMatrix(mpz_Matrix* c, mpz_Matrix* a, mpz_Matrix* b);

int mpz_canMatrixMul(mpz_Matrix* a, mpz_Matrix* b);
int mpz_mulMatrix(mpz_Matrix* c, mpz_Matrix* a, mpz_Matrix* b);
int mpz_mulAndAddMatrix(mpz_Matrix* c, mpz_Matrix* a, mpz_Matrix* b);

#endif