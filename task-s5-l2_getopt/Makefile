TARGET= nucleotide_pack
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o,$(SRC))

DEPS = ../common/manipulations.o ../task-s2-gets_safe/safe_string.o ../task-l1-nucleotide_pack/bit_coding.o ../task-l1-nucleotide_pack/nucleotide_pack.o
#DEPS := $(filter-out ../task-l1-nucleotide_pack/main.o, $(DEPS) )
DEP_DIRS = ../common ../task-s2-gets_safe ../task-l1-nucleotide_pack

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
