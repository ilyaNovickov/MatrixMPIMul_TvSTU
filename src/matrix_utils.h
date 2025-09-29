#ifndef MATRIX_UTILSC
#define MATRIX_UTILSC

typedef struct Matrix
{
    int rows;
    int colms;
    int* matrix;
};

struct Matrix createMatrix(int rows, int colms);

void freeMatrix(struct Matrix* matrix);

void setMatrixAt(struct Matrix* matrix, int row, int colm, int value);

int getMatrixAt(struct Matrix* matrix, int row, int colm);

#endif 



