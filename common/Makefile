SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o,$(SRC))
DEP = ../task-s2-gets_safe/safe_string.o
DEP_DIRS = ../task-s2-gets_safe

.PHONY:  all clean $(DEP_DIRS)

CFLAGS=-Wall -ansi -pedantic $(DEP)

all: $(DEP_DIRS) $(OBJ)

%.o: %.c $(DEP)
	$(CC) $(CFLAGS) -c $^
#include $(wildcard *.d)

$(DEP_DIRS):
	$(MAKE) -C $@

clean:
	rm -rf $(TARGET) *.o *.d  *.orig
