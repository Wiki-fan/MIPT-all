SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o,$(SRC))
TARGET = $(patsubst %.c,%,$(SRC))
DEPS = #../common/manipulations.o ../task-s2-gets_safe/safe_string.o
DEP_DIRS = #../common ../task-s2-gets_safe

CC=gcc
CFLAGS=-Wall -ansi -pedantic  $(DEPS)
VALGRIND_OPTIONS += --leak-check=full --read-var-info=yes --track-origins=yes

.PHONY:  all clean $(DEP_DIRS) ans1

all: ans1

ans1:
	$(CC) $(CFLAGS) -lm -pthread -o $@ 1.c

valgrind:
	valgrind $(VALGRIND_OPTIONS) ./ans1

$(DEP_DIRS):
	$(MAKE) -C $@

clean:
	rm -rf $(TARGET) *.o *.d  *.orig
