TARGET= 3-client 3-server 2-server

DEPS = ../common/utils.o ../task-s2-gets_safe/safe_string.o
DEP_DIRS = ../common ../task-s2-gets_safe

.PHONY:  all clean valgrind_client valgrind_server $(DEP_DIRS)

CFLAGS += -Wall -pedantic $(DEPS) -ansi -D_GNU_SOURCE
VALGRIND_OPTIONS += --leak-check=full --read-var-info=yes --track-origins=yes

all: $(DEP_DIRS) $(TARGET)

3-client: 3-client.o $(DEPS)
	$(CC) $^ -o $@  -lm -pthread

3-server: 3-server.o $(DEPS)
	$(CC) -o $@ $^ -lm -pthread

2-server: 2.o $(DEPS)
	$(CC) -o $@ $^ -lm -pthread

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
	rm -rf $(TARGET) *.o *.d *.orig 3-server 3-client
