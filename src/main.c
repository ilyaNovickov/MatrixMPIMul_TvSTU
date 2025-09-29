#include <stdio.h>

#include "matrix_utils.h"

int main(int argc, char* argv[]) {

    int n = 4;
    int m = 3;

    struct Matrix mat = createMatrix(n, m);

    
        int c = 1;

    for (int colms = 0; colms < m; colms ++)
    {
        for (int rows = 0; rows < n; rows++)
        {
            setMatrixAt(&mat, rows, colms, c++);
        }
    }
    

    for (int colms = 0; colms < m; colms ++)
    {
        for (int rows = 0; rows < n; rows++)
        {
            printf("%i | ", getMatrixAt(&mat, rows, colms));
        }
        printf("\n");
    }

    freeMatrix(&mat);

    return 0;
}