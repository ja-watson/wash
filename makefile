CC=gcc
CFLAGS=-I.
DEPS = bootstrap.h
OBJ = main.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

wash: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
