#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <time.h>

#include <mpi.h>

#include "matrix_utils_f.h"

void getCPUCoords(int* resRow, int* resCol, MPI_Comm* cart_comm, int rank)
{
    int coords[2];
    MPI_Cart_coords(*cart_comm, rank, 2, coords);
    *resRow = coords[0];
    *resCol = coords[1];
}

void printMatrixF(MatrixF* matrix)
{
    for (int row = 0; row < matrix->rows; row++)
    {
        for (int col = 0; col < matrix->colms; col++)
        {
            printf("%f |", getMatrixFAt(matrix, row, col));
        }
        printf("\n");
    }
    printf("\n");
}

void InitSqMatrixes(MatrixF* a, MatrixF* b, int matrixSize)
{
    *a = createMatrixF(matrixSize, matrixSize);
    *b = createMatrixF(matrixSize, matrixSize);
    //fillMatrixFRandom(&A);
    //fillMatrixFRandom(&B);
    for (int i = 0; i < matrixSize*matrixSize; i++)
    {
        a->data[i] = i + 1;
        b->data[i] = i + 1;
    }
    printf("=== Matrix A ===\n");
    printMatrixF(a);
    printf("================\n");
    printf("=== Matrix B ===\n");
    printMatrixF(b);
    printf("================\n");
}

int main(int argc, char** argv)
{
    int rank;

    int size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double start = MPI_Wtime();//timer 1

    //step 1: init CPU matrix P

    //size of CPU matrix (q x q)
    int q = (int)sqrt(size);
    //checking q
    if (q * q != size)
    {
        if (rank == 0)
            printf("Count of CPUs have to be q^2");
        MPI_Finalize();
        return -1;
    }

    //create topology for CPU matrix
    int dims[2] = {q, q};//CPU matrix
    int periods[2] = {1, 1}; //borders for B moving
    //switcher where P matrix will be
    MPI_Comm cart_comm;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &cart_comm);

    /*
    //find coords for CPU (rank) in P (CPU matrix)
    int coords[2];
    MPI_Cart_coords(cart_comm, rank, 2, coords);
    int row = coords[0];
    int col = coords[1];
    */

    //generate matrix A and B
    MatrixF A, B;
    int N = 6;//size of matrix (N x N)

    if (rank == 0)
    {
        InitSqMatrixes(&A, &B, N);
    }

    //size of one block in each CPU
    int block_size = N / q;

    MatrixF localA = createMatrixF(block_size, block_size);
    MatrixF localB = createMatrixF(block_size, block_size);
    MatrixF localC = createMatrixF(block_size, block_size);
    
    if (rank == 0)
    {
        for (int proc = 0; proc < size; proc++)
        {
            int row;
            int col;
            getCPUCoords(&row, &col, &cart_comm, proc);

            //take block
            MatrixF tempA = createMatrixF(block_size, block_size);
            MatrixF tempB = createMatrixF(block_size, block_size);
        
            for (int i = 0; i < block_size; i++)
            {
                for (int j = 0; j < block_size; j++)
                {
                    float valA = getMatrixFAt(&A, row*block_size+i, col*block_size+j);
                    float valB = getMatrixFAt(&B, row*block_size+i, col*block_size+j);

                    setMatrixFAt(&tempA, i, j, valA);
                    setMatrixFAt(&tempB, i, j, valB);
                }
            }

            if (proc == 0)
            {
                localA = tempA;
                localB = tempB;
            }
            else
            {
                MPI_Send(tempA.data, block_size * block_size, MPI_FLOAT, proc, 0, cart_comm);
                MPI_Send(tempB.data, block_size * block_size, MPI_FLOAT, proc, 1, cart_comm);
                freeMatrixF(&tempA);
                freeMatrixF(&tempB);
            }
        }
    }
    else
    {
        MPI_Recv(localA.data, block_size*block_size, MPI_FLOAT, 0, 0, cart_comm, MPI_STATUS_IGNORE);
        MPI_Recv(localB.data, block_size * block_size, MPI_FLOAT, 0, 1, cart_comm, MPI_STATUS_IGNORE);
    }

    int row;
    int col;
    getCPUCoords(&row, &col, &cart_comm, 0);

    //Fox's do
    MatrixF tempA = createMatrixF(block_size, block_size);

    for (int stage = 0; stage < q; stage++)
    {
        int root = (row + stage) % q;//which A matrix send

        if (col == root)
        {
            for (int i = 0; i < block_size * block_size; i++)
            {
                tempA.data[i] = localA.data[i];

            }
        }
        //send A for row
        MPI_Bcast(tempA.data, block_size*block_size, MPI_FLOAT, root, cart_comm);

        mulAndAddMatrixF(&localC, &tempA, &localB);

        //shift for colums of B matrix
        int src, dst;
        MPI_Cart_shift(cart_comm, 0, -1, &src, &dst);
        MPI_Sendrecv_replace(localB.data, block_size*block_size, MPI_FLOAT, dst, 0, src, 0, cart_comm, MPI_STATUS_IGNORE);
    }

    freeMatrixF(&tempA);

    //get result to rank == 0 CPU
    MatrixF C;

    if (rank == 0)
    {
        C = createMatrixF(N, N);

        for (int proc = 0; proc < size; proc++)
        {
            int row;
            int col;
            getCPUCoords(&row, &col, &cart_comm, proc);

            if (proc == 0)
            {
                for (int i = 0; i < block_size; i++)
                {
                    for (int j = 0; j < block_size; j++)
                    {
                        float val = getMatrixFAt(&localC, i, j);
                        setMatrixFAt(&C, row*block_size + i, col*block_size + j, val);
                    }
                }
            }
            else
            {
                MatrixF tempC = createMatrixF(block_size, block_size);

                MPI_Recv(tempC.data, block_size*block_size, MPI_FLOAT, proc, 2, cart_comm, MPI_STATUS_IGNORE);

                for (int i = 0; i < block_size; i++)
                {
                    for (int j = 0; j < block_size; j++)
                    {
                        float val = getMatrixFAt(&tempC, i, j);
                    setMatrixFAt(&C, row*block_size + i, col*block_size + j, val);
                    }
                }

                freeMatrixF(&tempC);
            }
        }
    }
    else
    {
        MPI_Send(localC.data, block_size*block_size, MPI_FLOAT, 0, 2, cart_comm);
    }


    //end
    double end = MPI_Wtime();//timer 2

    if (rank == 0)
    {
        printf("Time %f seconds for rank = %i\n", end - start, rank);
    }

    //trash bin
    printf("=========\n");
    printMatrixF(&C);
    
    MPI_Finalize();
    freeMatrixF(&A);
    freeMatrixF(&B);
    freeMatrixF(&C);
    freeMatrixF(&localA);
    freeMatrixF(&localB);
    freeMatrixF(&localC);
    freeMatrixF(&tempA);

    return 0;
}