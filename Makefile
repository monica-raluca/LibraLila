CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -g
DEPS=lists.h hashtables.h login.h utils.h books_management.h structs.h 
OBJ=lists.o hashtables.o login.o books_management.o LibraLila.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

build: LibraLila

LibraLila: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

run_LibraLila: build
	./LibraLila

.PHONY: clean

clean:
	rm -f *.o LibraLila
