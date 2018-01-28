TARGET= test_bitwise_print
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o,$(SRC))

.PHONY:  all clean valgrind

CFLAGS+=-Wall -ansi -pedantic -lm

all: $(TARGET)

test_bitwise_print: $(OBJ)
	$(CC) -o $@ $^

valgrind:
	valgrind $(VALGRIND_OPTIONS) ./$(TARGET) ../test 1>std.out

%.o: %.c
	$(CC) $(CFLAGS) -c -MD $<
include $(wildcard *.d)

clean:
	rm -rf $(TARGET) *.o *.d  *.orig std.out
