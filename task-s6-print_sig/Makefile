TARGET= print_sig
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o,$(SRC))

CFLAGS=-Wall -pedantic -ansi

all: $(TARGET)

print_sig: print_sig.o
	$(CC) -o $@ $^ -lm

%.o: %.c
	$(CC) $(CFLAGS) -c -MD $<
include $(wildcard *.d)

clean:
	rm -rf $(TARGET) *.o *.d  *.orig
