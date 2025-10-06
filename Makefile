# ==== Общие настройки ====
CC = mpicc
SRC_DIR = src
BIN_DIR = bin
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)
DEPS = $(OBJ:.o=.d)
LIBS = -lgmp

# ==== Сборка RELEASE ====
CFLAGS = -Wall -O2 -std=c11
TARGET = $(BIN_DIR)/matrix_mpi

all: $(TARGET)

$(TARGET): $(OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# ==== Сборка DEBUG ====
DEBUG_TARGET = $(BIN_DIR)/matrix_mpi_debug
DEBUG_FLAGS = -Wall -g -O0 -std=c11

debug: CFLAGS := $(DEBUG_FLAGS)
debug: $(DEBUG_TARGET)

$(DEBUG_TARGET): $(OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# ==== Общие правила ====
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -MMD -c $< -o $@

-include $(DEPS)

# ==== Запуск ====
run: $(TARGET)
	mpirun -np 4 ./$(TARGET)

run_debug: $(DEBUG_TARGET)
	./$(DEBUG_TARGET)

clean:
	rm -rf $(BIN_DIR)

.PHONY: all debug run run_debug clean