TARGET= client server

DEPS = ../common/utils.o
DEP_DIRS = ../common

.PHONY:  all clean valgrind_client valgrind_server $(DEP_DIRS)

CFLAGS += -Wall -pedantic $(DEPS) -ansi -d _GNU_SOURCE
VALGRIND_OPTIONS += --leak-check=full --read-var-info=yes --track-origins=yes

all: $(DEP_DIRS) $(TARGET)

client: client.o $(DEPS)
	$(CC) $^ -o $@  -lm

server: server.o $(DEPS)
	$(CC) -o $@ $^ -lm

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
	rm -rf $(TARGET) *.o *.d *.orig
