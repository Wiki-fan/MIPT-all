TARGET= nucleotide_pack
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o,$(SRC))

CC=gcc
CFLAGS=-Wall -ansi -pedantic

all: $(TARGET)

nucleotide_pack: $(OBJ)
	$(MAKE) -C ../task-s2-gets_safe
	$(CC) -o $@ $^ ../task-s2-gets_safe/safe_string.o -lm

%.o: %.c
	$(CC) $(CFLAGS) -c -MD $<
include $(wildcard *.d)

clean:
	rm -rf $(TARGET) *.o *.d *.orig