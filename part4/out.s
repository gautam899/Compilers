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
	movq	$2, %r8
	movq	$3, %r9
	movq	$5, %r10
	imulq	%r9, %r10
	subq	%r10, %r8
	movq	$8, %r9
	movq	$3, %r10
	movq	%r9,%rax
	idivq	%r10
	movq	%rax, %r9
	addq	%r8, %r9
	movq	%r9, %rdi
	call	printint
	movl $0, %eax
	popq %rbp
	ret
