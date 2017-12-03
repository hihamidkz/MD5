CC := gcc
MPICC := mpicc
CFLAGS := -std=c99
LFLAGS := -lcrypto -lm

.PHONY: all
all: hash

hash: main.c
	$(CC) -o $@ $^ $(CFLAGS) $(LFLAGS)
	
phash: MPI_main.c
	$(MPICC) -o $@ $^ $(CFLAGS) $(LFLAGS)
	
.PHONY: clean
clean:
	rm hash
