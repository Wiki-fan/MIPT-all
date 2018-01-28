TARGET=server
SRC = timer_stuff.c net_stuff.c vector_impl.c server_stuff.c error_handling.c server_action_stuff.c utils.c
OBJ = $(patsubst %.c,%.o,$(SRC))

.PHONY:  all clean valgrind_server $(DEP_DIRS)

CFLAGS += -Wall -pedantic -pthread -std=c99
VALGRIND_OPTIONS += --leak-check=full --read-var-info=yes --track-origins=yes

all: $(DEP_DIRS) $(TARGET)

server: server.o $(OBJ)
	$(CC) -o $@ $^ -lm -std=c99 -pthread -lrt -g

%.o: %.c
	$(CC) $(CFLAGS) -c -g -MD $<
include $(wildcard *.d)

valgrind_server:
	valgrind $(VALGRIND_OPTIONS) ./server

clean:
	rm -rf $(TARGET) *.o *.d *.orig vgcore.*
