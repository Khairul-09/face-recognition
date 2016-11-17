ifndef BUILD
BUILD = debug
endif

ifeq ($(CC), cc)
CC = gcc
endif

ifeq ($(BUILD), release)
CFLAGS = -O3
else ifeq ($(BUILD), debug)
CFLAGS = -g -Wall
endif

ifeq ($(CC), gcc)
LFLAGS = -lm -lblas -llapacke -lstdc++
else ifeq ($(CC), icc)
CFLAGS += -D INTEL_MKL
LFLAGS = -lm -mkl
endif

FLAGS_MATRIX = -x c

INCS = src/database.h src/image.h src/image_entry.h src/matrix.h src/timing.h
OBJS = database.o image.o image_entry.o matrix.o pca.o lda.o ica.o timing.o
BINS = face-rec test-matrix test-image

all: config $(BINS)

config:
	$(info BUILD  = $(BUILD))
	$(info CC     = $(CC))
	$(info CFLAGS = $(CFLAGS))
	$(info LFLAGS = $(LFLAGS))

image.o: src/image.h src/image.c
	$(CC) -c $(CFLAGS) src/image.c -o $@

image_entry.o: src/image_entry.h src/image_entry.c
	$(CC) -c $(CFLAGS) src/image_entry.c -o $@

matrix.o: image.o src/matrix.h src/matrix.cu
	$(CC) -c $(CFLAGS) $(FLAGS_MATRIX) src/matrix.cu -o $@

database.o: image.o image_entry.o matrix.o src/database.h src/database.c
	$(CC) -c $(CFLAGS) src/database.c -o $@

timing.o: src/timing.h src/timing.cpp
	$(CXX) -c $(CFLAGS) src/timing.cpp -o $@

pca.o: matrix.o timing.o src/database.h src/pca.c
	$(CC) -c $(CFLAGS) src/pca.c -o $@

lda.o: matrix.o timing.o src/database.h src/lda.c
	$(CC) -c $(CFLAGS) src/lda.c -o $@

ica.o: matrix.o src/database.h src/ica.c
	$(CC) -c $(CFLAGS) src/ica.c -o $@

face-rec: image.o image_entry.o matrix.o database.o pca.o lda.o ica.o timing.o src/main.c
	$(CC) $(CFLAGS) image.o image_entry.o matrix.o timing.o database.o pca.o lda.o ica.o $(LFLAGS) src/main.c -o $@

test-image: image.o matrix.o src/test_image.c
	$(CC) $(CFLAGS) image.o matrix.o $(LFLAGS) src/test_image.c -o $@

test-matrix: matrix.o src/test_matrix.c
	$(CC) $(CFLAGS) matrix.o $(LFLAGS) src/test_matrix.c -o $@

test-cublas: src/matrix.h src/matrix.cu src/test_matrix.c
	nvcc -c src/matrix.cu -o matrix.o
	gcc -c src/test_matrix.c -o test_matrix.o
	g++ matrix.o test_matrix.o -lm -lcudart -lcublas -o $@

clean:
	rm -f *.o *.dat $(BINS)
	rm -rf test_images train_images
