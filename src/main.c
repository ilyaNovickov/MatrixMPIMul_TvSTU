#include <stdio.h>
#include <time.h>

#include "matrix_utils.h"
#include "matrix_utils_f.h"
//#include "gmp_matrix_utils.h"

int main(int argc, char** argv)
{
    srand((unsigned) time(NULL)); // Инициализация random seed

    Matrix mat = createMatrix(3, 3);
    fillMatrixRandom(&mat, -100, 100); // Заполнение от -100 до 100

    for (int row = 0; row < mat.rows; row++)
    {
        for (int col = 0; col < mat.colms; col++)
        {
            printf("%i |", getMatrixAt(&mat, row, col));
        }
        printf("\n");
    }
    printf("\n");

    MatrixF fmat = createMatrixF(3, 3);
    fillMatrixFRandom(&fmat); // Заполнение значениями от -1 до +1

    for (int row = 0; row < fmat.rows; row++)
    {
        for (int col = 0; col < fmat.colms; col++)
        {
            printf("%f |", getMatrixFAt(&fmat, row, col));
        }
        printf("\n");
    }
    printf("\n");

    freeMatrix(&mat);
    freeMatrixF(&fmat);
}

/*
//Тестирование gmp
int main(int argc, char* argv[]) {

    int n = 3;
    int m = 3;

    struct mpz_Matrix mat1 = mpz_createMatrix(n+1, m);
    struct mpz_Matrix mat2 = mpz_createMatrix(n, m+1);
    
    int c = 1;

    for (int rows = 0; rows < mat1.rows; rows++)
    {
        for (int colms = 0; colms < mat1.colms; colms ++)
        {
            mpz_t tmp;
            mpz_init_set_si(tmp, 2);
            mpz_setMatrixAt(&mat1, rows, colms, tmp);
            mpz_clear(tmp);
        }
    }
    c = 1;
    for (int rows = 0; rows < mat2.rows; rows++)
    {
        for (int colms = 0; colms < mat2.colms; colms ++)
        {
            mpz_t tmp;
            mpz_init_set_si(tmp, 2);
            mpz_setMatrixAt(&mat2, rows, colms, tmp);
            mpz_clear(tmp);
        }
    }
    
    for (int rows = 0; rows < mat1.rows; rows++)
    {
        for (int colms = 0; colms < mat1.colms; colms ++)
        {
            mpz_t temp;
            mpz_init(temp);
            mpz_getMatrixAt(&mat1, rows, colms, temp);
            gmp_printf("%Zd | ", temp);
            mpz_clear(temp);
        }
        gmp_printf("\n");
    }
    gmp_printf("+++++++++++++++++++++++++++++++++++++\n");
    
    for (int rows = 0; rows < mat2.rows; rows++)
    {
        for (int colms = 0; colms < mat2.colms; colms ++)
        {
            mpz_t temp;
            mpz_init(temp);
            mpz_getMatrixAt(&mat2, rows, colms, temp);
            gmp_printf("%Zd | ", temp);
            mpz_clear(temp);
        }
        printf("\n");
    }
    printf("\n");
    c = 1;
    //struct Matrix res = createMatrix(n, m);
    struct mpz_Matrix res = mpz_createMatrixForMul(&mat1, &mat2);
    
    for (int rows = 0; rows < n; rows++)
    {
        for (int colms = 0; colms < m; colms ++)
        {
            mpz_t tmpSet;
            mpz_init_set_si(tmpSet, 2);
            mpz_setMatrixAt(&res, rows, colms, tmpSet);
            mpz_clear(tmpSet);

            mpz_t temp;
            mpz_init(temp);
            mpz_getMatrixAt(&res, rows, colms, temp);
            gmp_printf("%Zd | ", temp);
            mpz_clear(temp);
        }
        gmp_printf("\n");
    }
    gmp_printf("\n");
    int result = mpz_mulMatrix(&res, &mat1, &mat2);
    for (int rows = 0; rows < res.rows; rows++)
    {
        for (int colms = 0; colms < res.colms; colms ++)
        {
            mpz_t temp;
            mpz_init(temp);
            mpz_getMatrixAt(&res, rows, colms, temp);
            gmp_printf("%Zd | ", temp);
            mpz_clear(temp);
        }
        gmp_printf("\n");
    }

    mpz_freeMatrix(&mat1);
    mpz_freeMatrix(&mat2);
    mpz_freeMatrix(&res);

    return 0;
}
*/

/*
//test usual matrixes
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

    int s[] = {2,2,300}; 

    return 0;
}
*/