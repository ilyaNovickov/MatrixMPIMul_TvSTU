CC = mpicc
CFLAGS = -Wall -g -O0 -std=c11
LIBS = -lm -lgmp

SRC = src/main.c src/matrix_utils_f.c
TARGET = bin/matrix_mpi_debug

all: $(TARGET)

$(TARGET): $(SRC)
	mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -rf bin