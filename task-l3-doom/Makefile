TARGET= client server
SRC = config_stuff.c game_stuff.c tty_stuff.c net_stuff.c
OBJ = $(patsubst %.c,%.o,$(SRC))

DEPS = ../common/utils.o
DEP_DIRS = ../common

.PHONY:  all clean valgrind $(DEP_DIRS)

CFLAGS += -Wall -pedantic $(DEPS) -ansi
VALGRIND_OPTIONS += --leak-check=full --read-var-info=yes --track-origins=yes

all: $(DEP_DIRS) $(TARGET)

client: client.o $(OBJ) $(DEPS)
	$(CC) -o $@ $^ -lm

server: server.o $(OBJ) $(DEPS)
	$(CC) -o $@ $^ -lm

%.o: %.c
	$(CC) $(CFLAGS) -c -MD $<
include $(wildcard *.d)

valgrind:
	valgrind $(VALGRIND_OPTIONS) ./$(TARGET) ../??? 1>std.out

$(DEP_DIRS):
	$(MAKE) -C $@

clean:
	rm -rf $(TARGET) *.o *.d *.orig
