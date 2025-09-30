#include <stdio.h>

#include "matrix_utils.h"

int main(int argc, char* argv[]) {

    int n = 3;
    int m = 3;

    struct Matrix mat1 = createMatrix(n+1, m);
    struct Matrix mat2 = createMatrix(n, m+1);
    
    int c = 1;

    for (int rows = 0; rows < mat1.rows; rows++)
    {
        for (int colms = 0; colms < mat1.colms; colms ++)
        {
            //setMatrixAt(&mat, rows, colms, c++);
            setMatrixAt(&mat1, rows, colms, c++);
        }
    }
    c = 1;
    for (int rows = 0; rows < mat2.rows; rows++)
    {
        for (int colms = 0; colms < mat2.colms; colms ++)
        {
            //setMatrixAt(&mat, rows, colms, c++);
            setMatrixAt(&mat2, rows, colms, c++);
        }
    }
    
    for (int rows = 0; rows < mat1.rows; rows++)
    {
        for (int colms = 0; colms < mat1.colms; colms ++)
        {
            printf("%i | ", getMatrixAt(&mat1, rows, colms));
        }
        printf("\n");
    }
    printf("+++++++++++++++++++++++++++++++++++++\n");
    
    for (int rows = 0; rows < mat2.rows; rows++)
    {
        for (int colms = 0; colms < mat2.colms; colms ++)
        {
            
            printf("%i | ", getMatrixAt(&mat2, rows, colms));
        }
        printf("\n");
    }
    printf("\n");
    c = 1;
    //struct Matrix res = createMatrix(n, m);
    struct Matrix res = createMatrixForMul(&mat1, &mat2);
    
    for (int rows = 0; rows < n; rows++)
    {
        for (int colms = 0; colms < m; colms ++)
        {
            setMatrixAt(&res, rows, colms, c++);
            printf("%i | ", getMatrixAt(&res, rows, colms));
        }
        printf("\n");
    }
    printf("\n");
    int esult = mulMatrix(&res, &mat1, &mat2);
    for (int rows = 0; rows < res.rows; rows++)
    {
        for (int colms = 0; colms < res.colms; colms ++)
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