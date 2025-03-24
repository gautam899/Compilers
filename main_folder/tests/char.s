	.text
	.data
	.globl	a
a:	.byte	0
	.data
	.globl	b
b:	.long	0
	.data
	.globl	c
c:	.byte	0
	.text
	.globl	main
	.type	main, %function
main:
	push	{fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	str	r0, [fp, #-8]
	mov	r4, #18
	ldr	r3, .L2+0
	strb	r4, [r3]
	ldr	r3, .L2+0
	ldrb	r4, [r3]
	mov	r0, r4
	bl	printint
	mov	r4, r0
	ldr	r3, .L2+0
	mov	r4, r3
	ldr	r3, .L2+4
	str	r4, [r3]
	ldr	r3, .L2+4
	ldr	r4, [r3]
	ldrb	r4, [r4]
	ldr	r3, .L2+8
	strb	r4, [r3]
	ldr	r3, .L2+8
	ldrb	r4, [r3]
	mov	r0, r4
	bl	printint
	mov	r4, r0
	mov	r4, #0
	mov	r0, r4
	b	L1
L1:
	sub	sp, fp, #4
	pop	{fp, pc}
	.align	2
.L2:
	.word a
	.word b
	.word c
.L3:
