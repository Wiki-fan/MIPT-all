TARGET= less
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o,$(SRC))

DEPS = ../common/manipulations.o ../common/utils.o ../task-s2-gets_safe/safe_string.o
DEP_DIRS = ../common ../task-s2-gets_safe

.PHONY:  all clean $(DEP_DIRS)

CFLAGS=-Wall -ansi -pedantic $(DEPS)

all: $(DEP_DIRS) $(TARGET)

less:  $(OBJ) $(DEPS)
	$(CC) -o $@ $^ -lm

%.o: %.c
	$(CC) $(CFLAGS) -c -MD $<
include $(wildcard *.d)

$(DEP_DIRS):
	$(MAKE) -C $@

clean:
	rm -rf $(TARGET) *.o *.d *.orig
