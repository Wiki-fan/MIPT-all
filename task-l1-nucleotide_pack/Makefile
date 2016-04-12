TARGET= nucleotide_pack
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o,$(SRC))

DEPS = ../common/utils.o ../task-s2-gets_safe/safe_string.o
DEP_DIRS = ../common ../task-s2-gets_safe

.PHONY:  all clean valgrind $(DEP_DIRS)

CFLAGS += -Wall -ansi -pedantic
VALGRIND_OPTIONS += --leak-check=full --read-var-info=yes --track-origins=yes --vgdb=yes

all: $(DEP_DIRS) $(TARGET)

nucleotide_pack: $(OBJ)
	$(CC) -o $@ $^ $(DEPS) -lm

%.o: %.c
	$(CC) $(CFLAGS) -c -MD $<
include $(wildcard *.d)

valgrind:
	#valgrind $(VALGRIND_OPTIONS) ./$(TARGET) pack ../seqdump.fasta mid.gfasta
	#valgrind $(VALGRIND_OPTIONS) ./$(TARGET) unpack mid.gfasta ans.fa
	valgrind $(VALGRIND_OPTIONS) ./$(TARGET) pack human1.fa mid.gfasta
	valgrind $(VALGRIND_OPTIONS) ./$(TARGET) unpack mid.gfasta ans.fasta

$(DEP_DIRS):
	$(MAKE) -C $@

clean:
	rm -rf $(TARGET) *.o *.d *.orig