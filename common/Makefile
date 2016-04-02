SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o,$(SRC))

.PHONY:  all clean

CFLAGS=-Wall -ansi -pedantic $(DEP)

all: $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $^
#include $(wildcard *.d)

clean:
	rm -rf $(TARGET) *.o *.d  *.orig
