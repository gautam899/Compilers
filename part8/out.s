	.text
.LC0:
	.string	"%d\n"
printint:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	nop
	leave
	ret

	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	.comm	i,8,8
	.comm	j,8,8
	.comm	k,8,8
	.comm	l,8,8
	movq	$6, %r8
	movq	%r8, i(%rip)
	movq	$12, %r8
	movq	%r8, j(%rip)
	movq	i(%rip), %r8
	movq	j(%rip), %r9
	cmpq	%r9, %r8
	jge	L1
	movq	i(%rip), %r8
	movq	%r8, %rdi
	call	printint
	movq	$2, %r8
	movq	%r8, k(%rip)
	movq	$5, %r8
	movq	%r8, l(%rip)
	movq	k(%rip), %r8
	movq	l(%rip), %r9
	cmpq	%r9, %r8
	jle	L3
	movq	k(%rip), %r8
	movq	%r8, %rdi
	call	printint
	jmp	L4
L3:
	movq	l(%rip), %r8
	movq	%r8, %rdi
	call	printint
L4:
	jmp	L2
L1:
	movq	j(%rip), %r8
	movq	%r8, %rdi
	call	printint
L2:
	movl	$0, %eax
	popq	%rbp
	ret
