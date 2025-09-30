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

inline struct Matrix createMatrixForMul(struct Matrix* a, struct Matrix* b)
{
    return createMatrix(a->rows, b->colms);
}


inline void freeMatrix(struct Matrix* matrix)
{
    free(matrix->matrix);
}

inline void setMatrixAt(struct Matrix* matrix, int row, int colm, int value)
{
    matrix->matrix[row * matrix->colms + colm] = value;
    //*(matrix->matrix + row*colm + colm) = value;
}

inline int getMatrixAt(struct Matrix* matrix, int row, int colm)
{
    return matrix->matrix[row * matrix->colms + colm];
    //return *(matrix->matrix + row*colm + colm);
}

inline int canMatrixAdd(struct Matrix* a, struct Matrix* b)
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

inline int canMatrixMul(struct Matrix* a, struct Matrix* b)
{
    return a->colms == b->rows;
}

int mulMatrix(struct Matrix* c, struct Matrix* a, struct Matrix* b)
{
if (!canMatrixMul(a, b))
    {
        return -1;
    }

    if ( (c->rows != a->rows) || (c->colms != b->colms) )
    {
        return -2;
    }

    for (int row = 0; row < c->rows; row++)
    {
        for (int colm = 0; colm < c->colms; colm++)
        {
            unsigned long summ = 0;

            for (int r = 0; r < a->colms; r++)
            {
                summ += getMatrixAt(a, row, r) * getMatrixAt(b, r, colm);
            }
            
            setMatrixAt(c, row, colm, summ);
        }
    }

    return 0;
}

/// @brief Multiply two matrix and summ each result matrix element to exiting result matrix
/// @param c 
/// @param a 
/// @param b 
/// @return 0 - success; -1 - matrix cant be multiply; -2 - c cant be result matrix 
int mulAndAddMatrix(struct Matrix* c, struct Matrix* a, struct Matrix* b)
{
    if (!canMatrixMul(a, b))
    {
        return -1;
    }

    if ( (c->rows != a->rows) || (c->colms != b->colms) )
    {
        return -2;
    }

    for (int row = 0; row < c->rows; row++)
    {
        for (int colm = 0; colm < c->colms; colm++)
        {
            for (int r = 0; r < a->colms; r++)
            {
                setMatrixAt(c, row, colm, getMatrixAt(c, row, colm) + getMatrixAt(a, row, r) * getMatrixAt(b, r, colm));
            }
        }
    }

    return 0;
}




