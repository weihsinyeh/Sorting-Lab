	.file	"popcount.c"
	.text
	.globl	popcount_nibble
	.type	popcount_nibble, @function
popcount_nibble:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -20(%rbp)
	movl	-20(%rbp), %eax
	shrl	%eax
	andl	$2004318071, %eax
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	subl	%eax, -20(%rbp)
	movl	-4(%rbp), %eax
	shrl	%eax
	andl	$2004318071, %eax
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	subl	%eax, -20(%rbp)
	movl	-4(%rbp), %eax
	shrl	%eax
	andl	$2004318071, %eax
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	subl	%eax, -20(%rbp)
	movl	-20(%rbp), %eax
	shrl	$4, %eax
	movl	%eax, %edx
	movl	-20(%rbp), %eax
	addl	%edx, %eax
	andl	$252645135, %eax
	movl	%eax, -20(%rbp)
	movl	-20(%rbp), %eax
	imull	$16843009, %eax, %eax
	movl	%eax, -20(%rbp)
	movl	-20(%rbp), %eax
	shrl	$24, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	popcount_nibble, .-popcount_nibble
	.globl	popcount_oct
	.type	popcount_oct, @function
popcount_oct:
.LFB1:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -20(%rbp)
	movl	-20(%rbp), %eax
	shrl	%eax
	andl	$-613566757, %eax
	movl	%eax, %edx
	movl	-20(%rbp), %eax
	subl	%edx, %eax
	movl	%eax, %edx
	movl	-20(%rbp), %eax
	shrl	$2, %eax
	andl	$1227133513, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	shrl	$3, %eax
	movl	%eax, %edx
	movl	-4(%rbp), %eax
	addl	%edx, %eax
	andl	$-954437177, %eax
	movl	%eax, %ecx
	movl	%ecx, %eax
	imulq	$68174085, %rax, %rax
	shrq	$32, %rax
	movl	%ecx, %edx
	subl	%eax, %edx
	shrl	%edx
	addl	%edx, %eax
	shrl	$5, %eax
	movl	%eax, %edx
	sall	$6, %edx
	subl	%eax, %edx
	movl	%ecx, %eax
	subl	%edx, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	popcount_oct, .-popcount_oct
	.section	.rodata
.LC0:
	.string	"%u\n"
	.align 8
.LC3:
	.string	"popcount use nibble avg time : %lf\n"
	.align 8
.LC4:
	.string	"popcount use oct avg time : %lf\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$96, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	$0, -80(%rbp)
	movq	$0, -72(%rbp)
	movl	$0, -84(%rbp)
	jmp	.L6
.L7:
	movl	-84(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-48(%rbp), %rax
	movq	%rax, %rsi
	movl	$1, %edi
	call	clock_gettime@PLT
	movl	-84(%rbp), %eax
	movl	%eax, %edi
	call	popcount_nibble
	leaq	-32(%rbp), %rax
	movq	%rax, %rsi
	movl	$1, %edi
	call	clock_gettime@PLT
	movq	-32(%rbp), %rax
	cvtsi2sdq	%rax, %xmm1
	movsd	.LC1(%rip), %xmm0
	mulsd	%xmm0, %xmm1
	movq	-24(%rbp), %rax
	cvtsi2sdq	%rax, %xmm0
	addsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rdx
	movq	-48(%rbp), %rax
	cvtsi2sdq	%rax, %xmm1
	movsd	.LC1(%rip), %xmm0
	mulsd	%xmm0, %xmm1
	movq	-40(%rbp), %rax
	cvtsi2sdq	%rax, %xmm0
	addsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rax
	subq	%rax, %rdx
	movq	%rdx, %rax
	movq	%rax, -64(%rbp)
	movq	-64(%rbp), %rax
	addq	%rax, -80(%rbp)
	leaq	-48(%rbp), %rax
	movq	%rax, %rsi
	movl	$1, %edi
	call	clock_gettime@PLT
	movl	-84(%rbp), %eax
	movl	%eax, %edi
	call	popcount_oct
	leaq	-32(%rbp), %rax
	movq	%rax, %rsi
	movl	$1, %edi
	call	clock_gettime@PLT
	movq	-32(%rbp), %rax
	cvtsi2sdq	%rax, %xmm1
	movsd	.LC1(%rip), %xmm0
	mulsd	%xmm0, %xmm1
	movq	-24(%rbp), %rax
	cvtsi2sdq	%rax, %xmm0
	addsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rdx
	movq	-48(%rbp), %rax
	cvtsi2sdq	%rax, %xmm1
	movsd	.LC1(%rip), %xmm0
	mulsd	%xmm0, %xmm1
	movq	-40(%rbp), %rax
	cvtsi2sdq	%rax, %xmm0
	addsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rax
	subq	%rax, %rdx
	movq	%rdx, %rax
	movq	%rax, -56(%rbp)
	movq	-56(%rbp), %rax
	addq	%rax, -72(%rbp)
	addl	$1, -84(%rbp)
.L6:
	cmpl	$9999999, -84(%rbp)
	jbe	.L7
	cvtsi2sdq	-80(%rbp), %xmm0
	movsd	.LC2(%rip), %xmm1
	divsd	%xmm1, %xmm0
	leaq	.LC3(%rip), %rdi
	movl	$1, %eax
	call	printf@PLT
	cvtsi2sdq	-72(%rbp), %xmm0
	movsd	.LC2(%rip), %xmm1
	divsd	%xmm1, %xmm0
	leaq	.LC4(%rip), %rdi
	movl	$1, %eax
	call	printf@PLT
	movl	$0, %eax
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L9
	call	__stack_chk_fail@PLT
.L9:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC1:
	.long	0
	.long	1104006501
	.align 8
.LC2:
	.long	0
	.long	1097011920
	.ident	"GCC: (Ubuntu 9.4.0-1ubuntu1~20.04.2) 9.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
