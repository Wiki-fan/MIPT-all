TARGET= hello_world
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o,$(SRC))

CC=gcc
CFLAGS=-Wextra -ansi -pedantic -lm

all: $(TARGET)

safe_gets_test: $(OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -MD $<
include $(wildcard *.d)

clean:
	rm -rf $(TARGET) *.o *.d
