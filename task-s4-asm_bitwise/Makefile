TARGET= asm_bitwise
SRC = asm_bitwise.s
OBJ = asm_bitwise.o

AS=as

all: $(TARGET)

asm_bitwise: asm_bitwise.s
	$(AS) -o $@ $^

clean:
	rm -rf $(TARGET) *.o