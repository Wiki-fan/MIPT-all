TARGET= wcsort
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o,$(SRC))

DEPS = ../common/utils.o
DEP_DIRS = ../common

.PHONY:  all clean valgrind $(DEP_DIRS)

CFLAGS+=-Wall -ansi -pedantic
VALGRIND_OPTIONS= --leak-check=full --read-var-info=yes --track-origins=yes

all: $(DEP_DIRS) $(TARGET)

wcsort: $(OBJ)
	$(CC) -o $@ $^ $(DEPS) -lm

valgrind:
	valgrind $(VALGRIND_OPTIONS) ./$(TARGET) test 1>std.out

%.o: %.c
	$(CC) $(CFLAGS) -c -MD $<
include $(wildcard *.d)

$(DEP_DIRS):
	$(MAKE) -C $@

clean:
	rm -rf $(TARGET) *.o *.d  *.orig vgcore.* std.err std.out
