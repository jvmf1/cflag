CC=gcc
CFLAGS=-Wall
NAME=flag

all: example

cmap.o: cmap.c
	$(CC) $(CFLAGS) $^ -c

$(NAME).o: $(NAME).c
	$(CC) $(CFLAGS) $^ -c

example: example.c $(NAME).o cmap.o
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o example
