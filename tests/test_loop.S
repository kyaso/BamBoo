	.file	"test_loop.c"
	.option nopic
	.attribute arch, "rv32i2p0_m2p0_a2p0_f2p0_d2p0_c2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-32
	sw	s0,28(sp)
	addi	s0,sp,32
	li	a5,100
	sw	a5,-20(s0)
	sw	zero,-24(s0)
	j	.L2
.L3:
	lw	a5,-24(s0)
	andi	a4,a5,0xff
	lw	a5,-20(s0)
	sb	a4,0(a5)
	lw	a5,-20(s0)
	addi	a5,a5,1
	sw	a5,-20(s0)
	lw	a5,-24(s0)
	addi	a5,a5,1
	sw	a5,-24(s0)
.L2:
	lw	a4,-24(s0)
	li	a5,99
	ble	a4,a5,.L3
.L4:
	j	.L4
	.size	main, .-main
	.ident	"GCC: (GNU) 8.3.0"
