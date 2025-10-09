#ifndef MATRIX_UTILS_FLOAT_H
#define MATRIX_UTILS_FLOAT_H

typedef struct {
    int rows;
    int colms;
    float* data;
} MatrixF;

MatrixF createMatrixF(int rows, int colms);
MatrixF createMatrixFForMul(MatrixF* a, MatrixF* b);

void freeMatrixF(MatrixF* matrix);
void setMatrixFAt(MatrixF* matrix, int row, int colm, float value);
float getMatrixFAt(MatrixF* matrix, int row, int colm);

int canMatrixFAdd(MatrixF* a, MatrixF* b);
int addMatrixF(MatrixF* c, MatrixF* a, MatrixF* b);

int canMatrixFMul(MatrixF* a, MatrixF* b);
int mulMatrixF(MatrixF* c, MatrixF* a, MatrixF* b);
int mulAndAddMatrixF(MatrixF* c, MatrixF* a, MatrixF* b);

void fillMatrixFRandom(MatrixF* matrix); // float [-1.0, +1.0]

#endif