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
	.comm	a,8,8
	.comm	b,8,8
	.comm	c,8,8
	.comm	d,8,8
	movq	$20, %r8
	movq	$30, %r9
	movq	$4, %r10
	imulq	%r9, %r10
	addq	%r8, %r10
	movq	%r10, a(%rip)
	movq	$20, %r8
	movq	%r8, b(%rip)
	movq	$30, %r8
	movq	%r8, c(%rip)
	movq	a(%rip), %r8
	movq	%r8, %rdi
	call	printint
	movl	$0, %eax
	popq	%rbp
	ret
