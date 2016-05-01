TARGET= nucleotide_pack
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o,$(SRC))

DEPS = ../common/utils.o
#DEPS := $(filter-out ../task-l1-nucleotide_pack/main.o, $(DEPS) )
DEP_DIRS = ../common

.PHONY:  all clean $(DEP_DIRS)

CFLAGS=-Wall -ansi -pedantic $(DEPS)

all: $(DEP_DIRS) $(TARGET)

nucleotide_pack:  $(OBJ) $(DEPS)
	$(CC) -o $@ $^ -lm

%.o: %.c
	$(CC) $(CFLAGS) -c -MD $<
#include $(wildcard *.d)

$(DEP_DIRS):
	$(MAKE) -C $@

clean:
	rm -rf $(TARGET) *.o *.d *.orig
