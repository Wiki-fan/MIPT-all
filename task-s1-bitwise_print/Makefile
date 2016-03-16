TARGET= test_bitwise_print
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o,$(SRC))

CC=gcc
CFLAGS=-Wall -ansi -pedantic -lm

all: $(TARGET)

test_bitwise_print: $(OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -MD $<
include $(wildcard *.d)

clean:
	rm -rf $(TARGET) *.o *.d