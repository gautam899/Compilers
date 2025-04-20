	.text
	.data
	.globl	c
c:	.byte	0
	.data
	.globl	str
str:	.long	0
L2:
	.byte	72
	.byte	101
	.byte	108
	.byte	108
	.byte	111
	.byte	32
	.byte	119
	.byte	111
	.byte	114
	.byte	108
	.byte	100
	.byte	0
	.text
	.globl	main
	.type	main, %function
main:
	push	{fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	str	r0, [fp, #-8]
	mov	r4, #10
	ldr	r3, .L2+0
	strb	r4, [r3]
	ldr	r3, .L2+0
	ldrb	r4, [r3]
	mov	r0, r4
	bl	printint
	mov	r4, r0
	ldr     r4, =L2
        ldr     r3, =str
        str     r4, [r3]
	b	L3
L4:
	ldr	r3, .L2+4
	ldr	r4, [r3]
	ldrb	r4, [r4]
	mov	r0, r4
	bl	printchar
	mov	r4, r0
	ldr	r3, .L2+4
	ldr	r4, [r3]
	mov	r5, #1
	add	r5, r4, r5
	ldr	r3, .L2+4
	str	r5, [r3]
L3:
	ldr	r3, .L2+4
	ldr	r4, [r3]
	ldrb	r4, [r4]
	mov	r5, #0
	cmp	r4, r5
	bne	L4
	mov	r4, #0
	mov	r0, r4
	b	L1
L1:
	sub	sp, fp, #4
	pop	{fp, pc}
	.align	2
.L2:
	.word c
	.word str
.L3:
