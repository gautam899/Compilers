	.text
	.data
	.globl	a
a:	.long	0
	.data
	.globl	b
b:	.long	0
	.data
	.globl	c
c:	.long	0
	.data
	.globl	d
d:	.long	0
	.data
	.globl	e
e:	.long	0
	.text
	.globl	main
	.type	main, %function
main:
	push	{fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	str	r0, [fp, #-8]
	mov	r4, #2
	ldr	r3, .L2+0
	str	r4, [r3]
	mov	r4, #4
	ldr	r3, .L2+4
	str	r4, [r3]
	mov	r4, #3
	ldr	r3, .L2+8
	str	r4, [r3]
	mov	r4, #2
	ldr	r3, .L2+12
	str	r4, [r3]
	ldr	r3, .L2+0
	ldr	r4, [r3]
	ldr	r3, .L2+4
	ldr	r5, [r3]
	add	r5, r4, r5
	ldr	r3, .L2+8
	ldr	r4, [r3]
	ldr	r3, .L2+12
	ldr	r6, [r3]
	add	r6, r4, r6
	mul	r6, r5, r6
	ldr	r3, .L2+16
	str	r6, [r3]
	ldr	r3, .L2+16
	ldr	r4, [r3]
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
	.word d
	.word e
.L3:
