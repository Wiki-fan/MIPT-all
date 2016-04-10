TARGET= print_sig
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o,$(SRC))

.PHONY:  all clean valgrind

CFLAGS += -Wall -pedantic -ansi
VALGRIND_OPTIONS += --leak-check=full --read-var-info=yes --track-origins=yes

all: $(TARGET)

print_sig: print_sig.o
	$(CC) -o $@ $^ -lm

%.o: %.c
	$(CC) $(CFLAGS) -c -MD $<
include $(wildcard *.d)

valgrind:
	valgrind $(VALGRIND_OPTIONS) ./$(TARGET) 1>std.out

clean:
	rm -rf $(TARGET) *.o *.d  *.orig
