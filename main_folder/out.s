	.text
	.text
	.globl	fred
	.type	fred, %function
fred:
	push	{fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	str	r0, [fp, #-8]
	mov	r4, #20
	mov	r0, r4
	b	L1
L1:
	sub	sp, fp, #4
	pop	{fp, pc}
	.align	2
	.comm	result,4,4
	.text
	.globl	main
	.type	main, %function
main:
	push	{fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	str	r0, [fp, #-8]
	mov	r5, #10
	mov	r0, r5
	bl	printint
	mov	r5, r0
	mov	r4, #15
	mov	r0, r4
	bl	fred
	mov	r4, r0
	ldr	r3, .L2+0
	str	r4, [r3]
	ldr	r3, .L2+0
	ldr	r4, [r3]
	mov	r0, r4
	bl	printint
	mov	r4, r0
	mov	r4, #15
	mov	r0, r4
	bl	fred
	mov	r4, r0
	mov	r5, #10
	add	r5, r4, r5
	mov	r0, r5
	bl	printint
	mov	r5, r0
L2:
	sub	sp, fp, #4
	pop	{fp, pc}
	.align	2
.L2:
	.word result
.L3:
