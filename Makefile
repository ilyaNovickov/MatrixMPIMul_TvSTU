# Компилятор и флаги
CC = mpicc
CFLAGS = -Wall -Wextra -O2 -std=c11

# Пути
SRC_DIR = src
BIN_DIR = bin

# Файлы
TARGET = $(BIN_DIR)/matrix_mpi
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)
DEPS = $(OBJ:.o=.d)

# Основная цель по умолчанию
all: $(TARGET)

# Рецепт сборки целевого файла
$(TARGET): $(OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# Создание каталога bin при необходимости
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Компиляция каждого .c в .o и одновременное создание .d (файлов зависимостей)
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -MMD -c $< -o $@

# Включаем зависимости автоматически созданных .d файлов
-include $(DEPS)

# Запуск с 4 процессами
run: $(TARGET)
	mpirun -np 4 ./$(TARGET)

# Очистка
clean:
	rm -rf $(BIN_DIR)

# Упрощённое тестирование одной цели (например, утилиты)
test: run

.PHONY: all run clean test