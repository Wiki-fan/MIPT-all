TARGET= bitwise_print
SRC = $(wildcard *.c *.h)

CC=gcc
CFLAGS=-Wextra -ansi -pedantic -lm

all: $(TARGET)

bitwise_print: $(SRC)
	$(CC) $(COMPILER_OPTIONS) -o $@ $^

clean:
	rm -rf $(TARGET) *.o