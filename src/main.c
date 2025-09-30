#include <stdio.h>

#include "matrix_utils.h"

int main(int argc, char* argv[]) {

    int n = 4;
    int m = 3;

    struct Matrix mat1 = createMatrix(n, m);
    struct Matrix mat2 = createMatrix(n, m);
    
    //int c = 1;

    for (int rows = 0; rows < n; rows++)
    {
        for (int colms = 0; colms < m; colms ++)
        {
            //setMatrixAt(&mat, rows, colms, c++);
            setMatrixAt(&mat1, rows, colms, 2);
            setMatrixAt(&mat2, rows, colms, 2);
        }
    }
    
    for (int rows = 0; rows < n; rows++)
    {
        for (int colms = 0; colms < m; colms ++)
        {
            printf("%i | ", getMatrixAt(&mat1, rows, colms));
        }
        printf("\n");
    }
    printf("+++++++++++++++++++++++++++++++++++++\n");
    for (int rows = 0; rows < n; rows++)
    {
        for (int colms = 0; colms < m; colms ++)
        {
            printf("%i | ", getMatrixAt(&mat2, rows, colms));
        }
        printf("\n");
    }

    struct Matrix res = createMatrix(n, m);
    for (int rows = 0; rows < n; rows++)
    {
        for (int colms = 0; colms < m; colms ++)
        {
            printf("%i | ", getMatrixAt(&res, rows, colms));
        }
        printf("\n");
    }
    printf("\n");
    addMatrix(&res, &mat1, &mat2);
    for (int rows = 0; rows < n; rows++)
    {
        for (int colms = 0; colms < m; colms ++)
        {
            printf("%i | ", getMatrixAt(&res, rows, colms));
        }
        printf("\n");
    }

    freeMatrix(&mat1);
    freeMatrix(&mat2);
    freeMatrix(&res);

    return 0;
}