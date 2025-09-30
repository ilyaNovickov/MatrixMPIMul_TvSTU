#ifndef MATRIX_UTILSC
#define MATRIX_UTILSC

typedef struct Matrix
{
    int rows;
    int colms;
    int* matrix;
} Matrix;

struct Matrix createMatrix(int rows, int colms);

struct Matrix createMatrixForMul(struct Matrix* a, struct Matrix* b);


void freeMatrix(struct Matrix* matrix);

void setMatrixAt(struct Matrix* matrix, int row, int colm, int value);

int getMatrixAt(struct Matrix* matrix, int row, int colm);

int canMatrixAdd(struct Matrix* a, struct Matrix* b);

int addMatrix(struct Matrix* c, struct Matrix* a, struct Matrix* b);

int canMatrixMul(struct Matrix* a, struct Matrix* b);

int mulMatrix(struct Matrix* c, struct Matrix* a, struct Matrix* b);

int mulAndAddMatrix(struct Matrix* c, struct Matrix* a, struct Matrix* b);

#endif 



