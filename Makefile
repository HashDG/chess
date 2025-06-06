CC=gcc
EXE=chess

CFLAGS=-lm -std=c99

all: chess.o coup.o
	$(CC) $(CFLAGS) -o $(EXE) $^
	
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o $(EXE)
