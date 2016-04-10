TARGET= test_asm_bitwise
SRC = asm_bitwise.s
OBJ = asm_bitwise.o

.PHONY:  all clean valgrind

CFLAGS += -Wall -ansi -pedantic -lm -m32
VALGRIND_OPTIONS += --leak-check=full --read-var-info=yes --track-origins=yes

AS=as
ASFLAGS=--32

all: $(TARGET)

#ld $@.o -o $@ -e main
#$(AS) $(ASFLAGS) -o $@ $^

%.o: %.s
	$(AS) $(ASFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $^

test_asm_bitwise: main.c asm_bitwise.o
	$(CC) $(CFLAGS) -o $@ $^

valgrind:
	valgrind $(VALGRIND_OPTIONS) ./$(TARGET) 1>std.out

clean:
	rm -rf $(TARGET) *.o  *.d *.orig

