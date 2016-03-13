	.file	"main.c"
	.section	.rodata
.LC0:
	.string	"%4x"
	.text
	.globl	print
	.type	print, @function
print:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movl	$7, -4(%rbp)
	jmp	.L2
.L8:
	movl	$7, -8(%rbp)
	jmp	.L3
.L4:
	movq	-32(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -32(%rbp)
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	leaq	-24(%rbp), %rcx
	addq	%rcx, %rdx
	movzbl	(%rdx), %edx
	movzbl	%dl, %esi
	movl	-8(%rbp), %edx
	movl	%edx, %ecx
	sarl	%cl, %esi
	movl	%esi, %edx
	andl	$1, %edx
	movb	%dl, (%rax)
	subl	$1, -8(%rbp)
.L3:
	cmpl	$3, -8(%rbp)
	jg	.L4
	movq	-32(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -32(%rbp)
	movb	$32, (%rax)
	jmp	.L5
.L6:
	movq	-32(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -32(%rbp)
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	leaq	-24(%rbp), %rcx
	addq	%rcx, %rdx
	movzbl	(%rdx), %edx
	movzbl	%dl, %esi
	movl	-8(%rbp), %edx
	movl	%edx, %ecx
	sarl	%cl, %esi
	movl	%esi, %edx
	andl	$1, %edx
	movb	%dl, (%rax)
	subl	$1, -8(%rbp)
.L5:
	cmpl	$0, -8(%rbp)
	jns	.L6
	cmpl	$0, -4(%rbp)
	je	.L7
	movq	-32(%rbp), %rax
	movl	$2128928, (%rax)
.L7:
	subl	$1, -4(%rbp)
.L2:
	cmpl	$0, -4(%rbp)
	jns	.L8
	movq	-32(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -32(%rbp)
	movb	$10, (%rax)
	movl	$15, -4(%rbp)
	jmp	.L9
.L14:
	movl	-4(%rbp), %eax
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	cltq
	leaq	-24(%rbp), %rdx
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movzbl	%al, %eax
	movl	-4(%rbp), %edx
	andl	$1, %edx
	testl	%edx, %edx
	jne	.L10
	movl	$0, %edx
	jmp	.L11
.L10:
	movl	$4, %edx
.L11:
	movl	%edx, %ecx
	sarl	%cl, %eax
	andl	$15, %eax
	movl	%eax, %edx
	movq	-32(%rbp), %rax
	movl	$.LC0, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	sprintf
	movl	-4(%rbp), %eax
	andl	$1, %eax
	testl	%eax, %eax
	jne	.L12
	movq	-32(%rbp), %rax
	movl	$2105376, (%rax)
	jmp	.L13
.L12:
	movl	$32, %edi
	call	putchar
.L13:
	subl	$1, -4(%rbp)
.L9:
	cmpl	$0, -4(%rbp)
	jns	.L14
	movq	-32(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -32(%rbp)
	movb	$10, (%rax)
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	print, .-print
	.globl	print2
	.type	print2, @function
print2:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movl	$63, -4(%rbp)
	jmp	.L16
.L19:
	movq	-32(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -32(%rbp)
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	leaq	-24(%rbp), %rcx
	addq	%rcx, %rdx
	movzbl	(%rdx), %edx
	movzbl	%dl, %esi
	movl	-8(%rbp), %edx
	movl	%edx, %ecx
	sarl	%cl, %esi
	movl	%esi, %edx
	andl	$1, %edx
	testl	%edx, %edx
	je	.L17
	movl	$49, %edx
	jmp	.L18
.L17:
	movl	$48, %edx
.L18:
	movb	%dl, (%rax)
	subl	$1, -4(%rbp)
.L16:
	cmpl	$0, -4(%rbp)
	jns	.L19
	movq	-32(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -32(%rbp)
	movb	$10, (%rax)
	movq	-32(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -32(%rbp)
	movb	$10, (%rax)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	print2, .-print2
	.ident	"GCC: (GNU) 5.3.0"
	.section	.note.GNU-stack,"",@progbits
