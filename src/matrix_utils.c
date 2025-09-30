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

int canMatrixAdd(struct Matrix* a, struct Matrix* b)
{
    return (a->rows == b->rows) && (a->colms == b->colms);
}

/// @brief summing matixes
/// @param c - result matrix
/// @param a - operant a
/// @param b - operant b
/// @return 0 - success; -1 - diff size of matrix; -2 - "c" can't be result
int addMatrix(struct Matrix* c, struct Matrix* a, struct Matrix* b)
{
    if (!canMatrixAdd(a, b))
    {
        return -1;
    }

    if ( (c->rows != a->rows) || (c->colms != a->colms) )
    {
        return -2;
    }

    for (int row = 0; row < a->rows; row++)
    {
        for (int colm = 0; colm < a->colms; colm++)
        {
            setMatrixAt(c, row, colm, getMatrixAt(a, row, colm) + getMatrixAt(b, row, colm));
        }
    }

    return 0;
}

int mulMatrix(struct Matrix* c, struct Matrix* a, struct Matrix* b)
{
    return 0;
}




