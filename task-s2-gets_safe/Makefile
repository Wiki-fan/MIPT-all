TARGET= test_safe_gets
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o,$(SRC))
DEPS = ../common/manipulations.o ../common/utils.o
DEP_DIRS = ../common

.PHONY:  all clean $(DEP_DIRS)

CFLAGS=-Wall -ansi -pedantic -lm

all: $(DEP_DIRS) $(TARGET)

test_safe_gets: $(OBJ) $(DEPS)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -MD $<
include $(wildcard *.d)

$(DEP_DIRS):
	$(MAKE) -C $@

clean:
	rm -rf $(TARGET) *.o *.d  *.orig
