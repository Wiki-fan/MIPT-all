.code32


.data
format_str:
    .string "%4x "
byte_delim:
    .string " | "


.text
.globl print_bits

.type print_bits, @function
print_bits:

    movl 4(%esp), %eax # eax - value that we should print
    movl 8(%esp),%ebx # ebx - array pointer
    movl $32, %ecx # ecx - counter
    # edi - result of and
    push %eax # remember our input digit
    
    jmp ne4 # skip first comparison  because we don't need preceding whitespace
cycle:

    # check if ' | ' needed
    movl %ecx, %edi
    and $0x00000007, %edi  # checking last 3 binary digits to be 0 (integer will divide by 8)
    cmp $0, %edi
    jne ne8
    movl $0x20, (%ebx) # whitespace
    inc %ebx
    movl $0x7C, (%ebx) # |
    inc %ebx
    movl $0x20, (%ebx) # whitespace
    inc %ebx
    jmp ne4
ne8:
    # checking last 3 binary digits to be 0 (integer will divide by 4)
    and $0x00000003, %edi
    cmp $0, %edi
    jne ne4
    movl $0x20, (%ebx) # whitespace
    inc %ebx
ne4:  

    # retrieving  and writing actual bit
    movl %eax, %edi
    shr $31, %edi 
    andl $0x00000001, %edi  # retrieving bit
    cmp $1, %edi
    jne write0
    movl $0x31, (%ebx) # write '1' to str
    jmp write1
write0:
    movl $0x30, (%ebx) # write '0' to str
write1:
    
    shl $1, %eax # shift digit
    inc %ebx # go to new str element
    /*dec %ecx
    
    cmp $0, %ecx
    jne cycle*/
    loop cycle
    
    movl $0x0A, (%ebx) # newline
    inc %ebx
    
    pop %eax # resetting initial value
    movl $8, %ecx
    jmp ne2
    # cycle that prints hexadecimal values
hex_cycle:
    # append delimiters if needed
    movl %ecx, %edi
    andl $1, %edi
    cmp  $0, %edi # check division by 2
    jne ne2
    movl $0x7C, (%ebx) # |
    inc %ebx
    movl $0x20, (%ebx) # whitespace
    inc %ebx
ne2:    
    # getting actual 4 bit value
    movl %eax, %edi
    shr $28, %edi
    andl $0xF, %edx
    ## prolog 
    pushl %eax
    pushl %ecx
    pushl %ebx
    pushl %edi # push old state of registers
    ## end prolog 
    push %edi
    push $format_str
    push %ebx
    call sprintf
    addl $12, %esp
    ## epilog 
    popl %edi
    popl %ebx
    popl %ecx
    popl %eax
    addl $5, %ebx # move str pointer
    ## end epilog 
    
    shl $4, %eax
    loop hex_cycle
    
    movl $0x0, (%ebx) # end of string

    movl $0, %eax # return value 0
    ret
