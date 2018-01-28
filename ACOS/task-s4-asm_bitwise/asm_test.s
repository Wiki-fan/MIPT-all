	.file	"asm_test.c"
	.section	.rodata
.LC0:
	.string	"%d %d %d %d\n"
	.text
	.globl	printsmth
	.type	printsmth, @function
printsmth:
.LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$8, %esp
	subl	$4, %esp
	pushl	12(%ebp)
	pushl	8(%ebp)
	pushl	$.LC0
	call	printf
	addl	$16, %esp
	movl	$13, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	printsmth, .-printsmth
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	leal	4(%esp), %ecx
	.cfi_def_cfa 1, 0
	andl	$-16, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	.cfi_escape 0x10,0x5,0x2,0x75,0
	movl	%esp, %ebp
	pushl	%ecx
	.cfi_escape 0xf,0x3,0x75,0x7c,0x6
	subl	$20, %esp
	movl	$10, -12(%ebp)
	movl	$11, -16(%ebp)
	subl	$8, %esp
	pushl	-16(%ebp)
	pushl	-12(%ebp)
	call	printsmth
	addl	$16, %esp
	movl	%eax, -20(%ebp)
	movl	$0, %eax
	movl	-4(%ebp), %ecx
	.cfi_def_cfa 1, 0
	leave
	.cfi_restore 5
	leal	-4(%ecx), %esp
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (GNU) 5.3.0"
	.section	.note.GNU-stack,"",@progbits
