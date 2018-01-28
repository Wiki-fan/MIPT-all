TARGET= mysh
TEST= test

SRC = proc_stuff.c parser_stuff.c error_stuff.c
OBJ = $(patsubst %.c,%.o,$(SRC))
TEST_SRC = tests.c
TEST_OBJ = $(patsubst %.c,%.o,$(TEST_SRC))

DEPS = ../common/utils.o
DEP_DIRS = ../common

.PHONY:  all clean test valgrind $(DEP_DIRS)

CFLAGS += -Wall -pedantic -ansi
VALGRIND_OPTIONS += --leak-check=full --read-var-info=yes --track-origins=yes --show-leak-kinds=all

all: $(DEP_DIRS) $(TARGET) $(TEST)

$(TARGET): $(SRC) main.o $(DEPS)
	$(CC) $(CFLAGS) -o $@ $^ -lm
$(TEST): $(SRC) tests.o $(DEPS)
	$(CC) $(CFLAGS) -o $@ $^ -lm

%.o: %.c
	$(CC) $(CFLAGS) -c -MD $<
include $(wildcard *.d)

valgrind:
	valgrind $(VALGRIND_OPTIONS) ./$(TARGET)

$(DEP_DIRS):
	$(MAKE) -C $@

clean:
	rm -rf $(TARGET) $(TEST) *.o *.d *.orig vgcore.*
