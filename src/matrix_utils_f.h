#ifndef MATRIX_UTILS_FLOAT_H
#define MATRIX_UTILS_FLOAT_H

/// @brief Структура матрицы для чисел float. 
///Содержит указатель на массив float
typedef struct {
    int rows;
    int colms;
    float* data;
} MatrixF;

/// @brief Создать матрицу заданного размера
/// @param rows Кол-во строк
/// @param colms Кол-во столбцов
/// @return Структура матрицы, где массив чисел находиться в динамической памяти
/// (требует очистки)
MatrixF createMatrixF(int rows, int colms);

/// @brief Создание матрицы результата перемножения заданных матриц
/// @param a Матрица A
/// @param b Матрица B
/// @return Если матрицы A и B можно перемножить, то получиться матрица C с динамическим массивом
///Иначе будет пустая матрица с массивом NULL
MatrixF createMatrixFForMul(MatrixF* a, MatrixF* b);

/// @brief Освободить память от массива матрицы
/// @param matrix Матрица на очистку
void freeMatrixF(MatrixF* matrix);

/// @brief Установить значение в элемент матрицы (обработка выхода за границы массив не предусмотрена)
/// @param matrix Целевая матрица
/// @param row Колона
/// @param colm Стоблец
/// @param value Новое значение
void setMatrixFAt(MatrixF* matrix, int row, int colm, float value);

/// @brief Получить элемент матрицы (обработка выхода за границы массив не предусмотрена)
/// @param matrix Целевая матрицы
/// @param row Строка
/// @param colm Колона
/// @return Значение элемента (копия)
float getMatrixFAt(MatrixF* matrix, int row, int colm);

/// @brief Определяет, можно ли суммировать матрицы
/// @param a Матрица A
/// @param b Матрица B
/// @return bool значение (в рамках чистого C)
int canMatrixFAdd(MatrixF* a, MatrixF* b);

/// @brief Суммировать матрицы между собой (A + B = C)
/// @param c Матрица результата C
/// @param a Матрица A
/// @param b Матрица B
/// @return -1 в случае, если нельзя суммировать матрицы, иначе 0
int addMatrixF(MatrixF* c, MatrixF* a, MatrixF* b);

/// @brief Определяет, можно ли перемножить матрицы
/// @param a Матрица A
/// @param b Матрица B
/// @return bool значение (в рамках чистого C) 
int canMatrixFMul(MatrixF* a, MatrixF* b);

/// @brief Умножение матриц (A * B = C)
/// @param c Матрциа результата C
/// @param a Матрциа A
/// @param b Матрица B
/// @return -1 в случае, если нельзя умножать матрицы, иначе 0
int mulMatrixF(MatrixF* c, MatrixF* a, MatrixF* b);

/// @brief Суммирование и накопление матриц (C += (A * B) )
/// @param c Матрица результата C
/// @param a Матрциа A
/// @param b Матрица B
/// @return -1 в случае, если нельзя умножать матрицы, иначе 0
int mulAndAddMatrixF(MatrixF* c, MatrixF* a, MatrixF* b);

/// @brief Заполнение матрицы случайными значениями в интервале [-1; +1]
/// @param matrix Матрица на заплнение
void fillMatrixFRandom(MatrixF* matrix); // float [-1.0, +1.0]

#endif