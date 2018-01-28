TARGET= less
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o,$(SRC))

DEPS = ../common/utils.o
DEP_DIRS = ../common

.PHONY:  all clean valgrind $(DEP_DIRS)

CFLAGS += -Wall  -pedantic $(DEPS) -ansi
VALGRIND_OPTIONS += --leak-check=full --read-var-info=yes --track-origins=yes

all: $(DEP_DIRS) $(TARGET)

less:  $(OBJ) $(DEPS)
	$(CC) -o $@ $^ -lm

%.o: %.c
	$(CC) $(CFLAGS) -c -MD $<
include $(wildcard *.d)

valgrind:
	valgrind $(VALGRIND_OPTIONS) ./$(TARGET) ../test 1>std.out

$(DEP_DIRS):
	$(MAKE) -C $@

clean:
	rm -rf $(TARGET) *.o *.d *.orig
