//Размер перемножаемых матриц (константа, изменить и перекомпилировать, иначе беда)
//РАЗМЕР МАТРИЦЫ И КОЛ-ВО CPU
//4x4 --- 2 --- ?
//6x6 --- 4 --- OK
//9x9 --- 9 --- OK
//10.000x10.000 --- 1000 --- mb, too slow
#define MATRIXSIZE 10000

//Определяет, выводить ли в консоль
//матрицы A, B, C
//Используется для дебага при малых матрицах (MATRIXSIZE)
#define CONSOLE 0

//Определяет, записываются ли матрицы в CSV файл,
//чтобы можно проанализировать данные
#define WRITECSV 0

//Определяет заполняет ли матрицы слючайными значениями (0)
//или порядковым номером элемента (1)
#define FILLMATRIXWITHVALS 0


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <time.h>

#include <mpi.h>

#include "matrix_utils_f.h"

/// @brief Определить координаты CPU в топологии
/// @param resRow Выходная строка
/// @param resCol Выходной столбец
/// @param cart_comm Топология
/// @param rank Ранг процессора (т. е. сам CPU)
void getCPUCoords(int* resRow, int* resCol, MPI_Comm* cart_comm, int rank)
{
    int coords[2];
    MPI_Cart_coords(*cart_comm, rank, 2, coords);
    *resRow = coords[0];
    *resCol = coords[1];
}

/// @brief Вывод матрицы
/// @param matrix Выодимая матрицы
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
    //printf("\n");
}

/// @brief Иницилизация матриц дял алгоритма фокса
/// @param a Матрицы A
/// @param b Матрицы B
/// @param matrixSize Размер квадратных матриц
void InitSqMatrixes(MatrixF* a, MatrixF* b, int matrixSize)
{
    *a = createMatrixF(matrixSize, matrixSize);
    *b = createMatrixF(matrixSize, matrixSize);

    #if FILLMATRIXWITHVALS == 0
    fillMatrixFRandom(a);
    fillMatrixFRandom(b);
    #else
    for (int i = 0; i < matrixSize*matrixSize; i++)
    {
        a->data[i] = i + 1;
        b->data[i] = i + 1;
    }
    #endif
}



int main(int argc, char** argv)
{
    int rank;

    int size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double start;
    if (rank == 0)
    {
        start = MPI_Wtime();
    }

    #pragma region BigInitRegion
    /*
    Шаг №0 - Иницилизация данных
    Пусть q - размер матрицы процессов
    block_size - размер блока в каждом CPU
    size - кол-во процессов (определяется через MPI)
    */
    #pragma region InitData

    //Размер матрицы процессоров (q x q)
    int q = (int)sqrt(size);

    //Проверка q
    if (q * q != size)
    {
        if (rank == 0)
            printf("Count of CPUs have to be q^2");
        MPI_Finalize();
        return -1;
    }

    //Создание матрицы для топологии CPU
    int dims[2] = {q, q};//CPU матрица
    int periods[2] = {1, 1}; //Границы для перемещения блоков матриц B
    //Коммутатор для работы с топологией
    MPI_Comm cart_comm;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &cart_comm);

    //Создание подкоммуникаторов для работы со строками
    MPI_Comm row_comm;
    int remain_dims[2] = {0, 1}; //Работа только со строками
    MPI_Cart_sub(cart_comm, remain_dims, &row_comm);
    
    int N = MATRIXSIZE;//size of matrix (N x N)

    //Размер блоков в каждом CPU
    int block_size = N / q;
    //int block_size = 3;

    #pragma region Values
    if (rank == 0)
    {
        printf("VALUES :\n");
        printf("SIZE == %i\n", size);
        printf("N == %i \\\\Square matrix size\n", N);
        printf("q == %i \\\\Size of CPU matrix\n", q);
        printf("-n == %i \\\\Block size\n\n\n", block_size);
    }
    #pragma endregion Values

    #pragma endregion InitData

    /*
    Шаг №0 (всё-ещё) - Отправка блоков по матрицам (первичная)

    CPU №0 делит матрицы A, B и C на блоки
    и рассылает на нужные процессоры
    */
    #pragma region InitBlocks
    MatrixF localA = createMatrixF(block_size, block_size);
    MatrixF localB = createMatrixF(block_size, block_size);
    MatrixF localC = createMatrixF(block_size, block_size);
    
    if (rank == 0)//CPU №0 формирует блоки
    {
        #pragma region SendingBlocks
        MatrixF A, B;
        //Иницилизация матриц размером N
        InitSqMatrixes(&A, &B, N);

        #if CONSOLE == 1
        printf("=== Matrix A ===\n");
        printMatrixF(&A);
        printf("================\n");
        printf("=== Matrix B ===\n");
        printMatrixF(&B);
        printf("================\n");
        #endif

        #if WRITECSV == 1
        writeMatrixFinFile("A.csv", &A);
        writeMatrixFinFile("B.csv", &B);
        #endif
    
        //Формирования блоков для каждого ЦП
        for (int proc = 0; proc < size; proc++)
        {
            int rowProc;
            int colProc;
            getCPUCoords(&rowProc, &colProc, &cart_comm, proc);

            //printf("CURRENT CPU ROW == %i and COL == %i\n", rowProc, colProc);

            MatrixF tempA = createMatrixF(block_size, block_size);
            MatrixF tempB = createMatrixF(block_size, block_size);
            
            //Вычленение блоков из матриц A и B  
            for (int i = 0; i < block_size; i++)
            {
                for (int j = 0; j < block_size; j++)
                {
                    float valA = getMatrixFAt(&A, rowProc*block_size+i, colProc*block_size+j);
                    float valB = getMatrixFAt(&B, rowProc*block_size+i, colProc*block_size+j);

                    setMatrixFAt(&tempA, i, j, valA);
                    setMatrixFAt(&tempB, i, j, valB);
                }
            }

            if (proc == 0)
            {
                //Мы уже на CPU №0
                localA = tempA;
                localB = tempB;

                //printf("SENDING BLOCKS\n");
                //printf("DESTANATION RANK == %i\n", proc);
                //printMatrixF(&localA);
            }
            else
            {
                //Отправка блоков 
                MPI_Send(tempA.data, block_size * block_size, MPI_FLOAT, proc, 0, cart_comm);
                MPI_Send(tempB.data, block_size * block_size, MPI_FLOAT, proc, 1, cart_comm);

                //printf("SENDING BLOCKS\n");
                //printf("DESTANATION RANK == %i\n", proc);
                //printMatrixF(&tempA);

                freeMatrixF(&tempA);
                freeMatrixF(&tempB);
            }
        }

        freeMatrixF(&A);
        freeMatrixF(&B);
        #pragma endregion SendingBlocks
    }
    else
    {
        //Если это не CPU №0 (главный), то CPU получает блоки матриц A и B
        MPI_Recv(localA.data, block_size*block_size, MPI_FLOAT, 0, 0, cart_comm, MPI_STATUS_IGNORE);
        MPI_Recv(localB.data, block_size * block_size, MPI_FLOAT, 0, 1, cart_comm, MPI_STATUS_IGNORE);

        //int rank;
        //MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        //printf("RECIEVE BLOCKS\n");
        //printf("RANK == %i\n", rank);
        //printMatrixF(&localA);
    }
    #pragma endregion InitBlocks

    #pragma endregion BigInitRegion

    //Оснвоной алгоритм Фокса
    #pragma region FoxAlgorithm

    //Получение координат текущего CPU
    int row;
    int col;
    MPI_Comm_rank(cart_comm, &rank);
    getCPUCoords(&row, &col, &cart_comm, rank);

    //Промежуточный блок A для отправки по строкам
    MatrixF tempA = createMatrixF(block_size, block_size);

    //Выполняется q-1 раз
    for (int stage = 0; stage < q; stage++)
    {
        int root = (row + stage) % q;//Определяет CPU, который отправляет блок A
        
        //Если мы в главном столбце на этом шаге, то
        //Копирование блока A для отправки
        if (col == root)
        {
            for (int i = 0; i < block_size * block_size; i++)
            {
                tempA.data[i] = localA.data[i];
            }
        }
        //Отправка блока A по строкам
        MPI_Bcast(tempA.data, block_size*block_size, MPI_FLOAT, root, row_comm);

        //Перемножение блоков A и B и накопление их суммы в блоке C
        mulAndAddMatrixF(&localC, &tempA, &localB);

        //Перемещение блоков B вверх по стоблам (циклически)
        int src, dst;// CPU, от которых мы получим новые данные и куда мы отправим свои данные
        MPI_Cart_shift(cart_comm, 0, -1, &src, &dst);//Ищет соседний CPU в топлогии по колонне (-1)
        //Одновременно отправляет данные в dst и получает новые данные от src
        MPI_Sendrecv_replace(localB.data, block_size*block_size, MPI_FLOAT, dst, 0, src, 0, cart_comm, MPI_STATUS_IGNORE);
    }

    freeMatrixF(&tempA);
    #pragma endregion FoxAlgorithm

    //Получение результатов на CPU №0
    #pragma region GetResult

    MatrixF C;

    //Сборка результата только на CPU №0
    if (rank == 0)
    {
        C = createMatrixF(N, N);

        //Проход по каждома ЦП
        for (int proc = 0; proc < size; proc++)
        {
            int row;
            int col;
            getCPUCoords(&row, &col, &cart_comm, proc);

            //ЦП №0 уже знает свои блок. Уже нужно просто скопировать
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
                //Для других ЦП: нужно получить блок C
                //и скопировать его в матрицу результата с учётом смещения
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
    //Другие CPU только отправляют свои блоки C на ЦП №0
    else
    {
        MPI_Send(localC.data, block_size*block_size, MPI_FLOAT, 0, 2, cart_comm);
    }

    #pragma endregion GetResult


    //trash bin
    #pragma region FreesAndEnd
    if (rank == 0)
    {
        #if CONSOLE == 1
        printf("=== Matrix C ===\n");
        printMatrixF(&C);
        printf("================\n");
        #endif

        #if WRITECSV == 1
        writeMatrixFinFile("C.csv", &C);
        #endif

        freeMatrixF(&C);

        double end = MPI_Wtime();
        printf("Time %f seconds (mb) for rank = %i\n", end - start, rank);
    
    }

    MPI_Finalize();

    freeMatrixF(&localA);
    freeMatrixF(&localB);
    freeMatrixF(&localC);

    return 0;
    #pragma endregion
}
