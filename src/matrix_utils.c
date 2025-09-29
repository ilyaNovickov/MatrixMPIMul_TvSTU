#include "matrix_utils.h"
#include <stdlib.h>



struct Matrix createMatrix(int rows, int colms)
{
    struct Matrix matrix;

    matrix.rows = rows;
    matrix.colms = colms;

    matrix.matrix = calloc(matrix.rows * matrix.colms,
         sizeof(int));
         
    return matrix;
}

void freeMatrix(struct Matrix* matrix)
{
    free(matrix->matrix);
}

void setMatrixAt(struct Matrix* matrix, int row, int colm, int value)
{
    matrix->matrix[row * matrix->colms + colm] = value;
    //*(matrix->matrix + row*colm + colm) = value;
}

int getMatrixAt(struct Matrix* matrix, int row, int colm)
{
    return matrix->matrix[row * matrix->colms + colm];
    //return *(matrix->matrix + row*colm + colm);
}




