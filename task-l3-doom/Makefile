TARGET= client server
SRC = config_stuff.c game_stuff.c tty_stuff.c net_stuff.c vector_impl.c data_stuff.c server_stuff.c client_stuff.c error_stuff.c server_action_stuff.c client_action_stuff.c
OBJ = $(patsubst %.c,%.o,$(SRC))

DEPS = ../common/utils.o
DEP_DIRS = ../common

.PHONY:  all clean valgrind $(DEP_DIRS)

CFLAGS += -Wall -pedantic -ansi -pthread
VALGRIND_OPTIONS += --leak-check=full --read-var-info=yes --track-origins=yes

all: $(DEP_DIRS) $(TARGET)

client: client.o $(OBJ) $(DEPS)
	$(CC) -o $@ $^ -lm -pthread -lrt

server: server.o $(OBJ) $(DEPS)
	$(CC) -o $@ $^ -lm -pthread -lrt

%.o: %.c
	$(CC) $(CFLAGS) -c -MD $<
include $(wildcard *.d)

valgrind_server:
	valgrind $(VALGRIND_OPTIONS) ./server

valgrind_client:
	valgrind $(VALGRIND_OPTIONS) ./client

$(DEP_DIRS):
	$(MAKE) -C $@

clean:
	rm -rf $(TARGET) *.o *.d *.orig vgcore.*
