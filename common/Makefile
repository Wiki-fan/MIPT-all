SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o,$(SRC))

CC=gcc
CFLAGS=-Wall -ansi -pedantic -lm

all: $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c -MD $<
include $(wildcard *.d)

clean:
	rm -rf $(TARGET) *.o *.d  *.orig