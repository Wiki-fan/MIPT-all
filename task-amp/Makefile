TARGET= server
SRC = timer_stuff.c net_stuff.c vector_impl.c server_stuff.c error_stuff.c server_action_stuff.c
OBJ = $(patsubst %.c,%.o,$(SRC))

DEPS = ../common/utils.o
DEP_DIRS = ../common

.PHONY:  all clean valgrind $(DEP_DIRS)

CFLAGS += -Wall -pedantic -ansi -pthread
VALGRIND_OPTIONS += --leak-check=full --read-var-info=yes --track-origins=yes

all: $(DEP_DIRS) $(TARGET)

server: server.o $(OBJ) $(DEPS)
	$(CC) -o $@ $^ -lm -pthread -lrt -g

%.o: %.c
	$(CC) $(CFLAGS) -c -g -MD $<
include $(wildcard *.d)

valgrind_server:
	valgrind $(VALGRIND_OPTIONS) ./server

$(DEP_DIRS):
	$(MAKE) -C $@

clean:
	rm -rf $(TARGET) *.o *.d *.orig vgcore.*
