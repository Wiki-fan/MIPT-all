SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o,$(SRC))
TARGET = $(patsubst %.c,%,$(SRC))
DEPS = ../common/manipulations.o ../task-s2-gets_safe/safe_string.o
DEP_DIRS = ../common ../task-s2-gets_safe

CC=gcc
CFLAGS=-Wall -ansi -pedantic  $(DEPS)

.PHONY:  all clean $(DEP_DIRS)

all: $(DEP_DIRS) $(TARGET)

%.o: %.c $(DEP_DIRS)
	$(CC) $(CFLAGS) -c -MD $<

#%: %.c  $(DEP_DIRS)
#	$(CC) $(CFLAGS) -MD $< -lm

$(DEP_DIRS):
	$(MAKE) -C $@

clean:
	rm -rf $(TARGET) *.o *.d  *.orig
