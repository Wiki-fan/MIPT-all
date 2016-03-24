TARGET= print_sig throw_sig
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o,$(SRC))

CC=gcc
CFLAGS=-Wall -pedantic -lm -O0

all: $(TARGET)

print_sig: print_sig.o
	$(CC) -o $@ $^

throw_sig: throw_sig.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -MD $<
include $(wildcard *.d)

clean:
	rm -rf $(TARGET) *.o *.d  *.orig