TARGET= parallel_matrix
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o,$(SRC))

.PHONY:  all clean valgrind

CFLAGS += -Wall -pedantic -ansi
VALGRIND_OPTIONS += --leak-check=full --read-var-info=yes --track-origins=yes

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ -lm

%.o: %.c
	$(CC) $(CFLAGS) -c -MD $<
include $(wildcard *.d)

valgrind:
	valgrind $(VALGRIND_OPTIONS) ./$(TARGET)

clean:
	rm -rf $(TARGET) *.o *.d  *.orig
