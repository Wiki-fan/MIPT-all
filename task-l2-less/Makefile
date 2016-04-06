TARGET= less
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o,$(SRC))

DEPS = ../common/utils.o
DEP_DIRS = ../common

.PHONY:  all clean $(DEP_DIRS)

CFLAGS=-Wall  -pedantic $(DEPS) -ansi

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
