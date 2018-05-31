.globl _devices
	.data
	.align 5
_devices:
	.word	-16384
	.word	2
	.long	_memory
	.word	-32766
	.word	2
	.long	0
	.word	-32767
	.word	1
	.long	0
	.space 488
	.text
.globl _device_read
_device_read:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$20, %esp
	call	L18
"L00000000001$pb":
L18:
	popl	%ebx
	movl	8(%ebp), %eax
	sall	$3, %eax
	movl	%eax, %edx
	leal	_devices-"L00000000001$pb"(%ebx), %eax
	leal	(%edx,%eax), %eax
	movl	%eax, -24(%ebp)
	movl	-24(%ebp), %eax
	movzwl	(%eax), %eax
	testw	%ax, %ax
	jns	L2
	cmpl	$0, 8(%ebp)
	je	L4
	andl	$-64, 12(%ebp)
L4:
	movl	12(%ebp), %eax
	sall	$12, %eax
	orl	%eax, 16(%ebp)
	movl	-24(%ebp), %eax
	movzwl	(%eax), %eax
	movzwl	%ax, %eax
	andl	$1, %eax
	testb	%al, %al
	je	L6
	movl	-24(%ebp), %eax
	movl	4(%eax), %eax
	movl	%eax, %edx
	movl	16(%ebp), %eax
	addl	%eax, %eax
	leal	(%edx,%eax), %eax
	movl	%eax, -16(%ebp)
	movl	-16(%ebp), %eax
	movzbl	(%eax), %eax
	movzbl	%al, %eax
	movl	%eax, %edx
	sall	$8, %edx
	movl	-16(%ebp), %eax
	movzbl	1(%eax), %eax
	movzbl	%al, %eax
	orl	%edx, %eax
	movl	%eax, -20(%ebp)
	jmp	L16
L6:
	movl	-24(%ebp), %eax
	movzwl	(%eax), %eax
	movzwl	%ax, %eax
	andl	$2, %eax
	testl	%eax, %eax
	je	L9
	movl	-24(%ebp), %eax
	movl	4(%eax), %eax
	movl	%eax, %ecx
	movl	16(%ebp), %edx
	movl	%edx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	leal	(%ecx,%eax), %eax
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	movzbl	(%eax), %eax
	movzbl	%al, %eax
	movl	%eax, %edx
	sall	$16, %edx
	movl	-12(%ebp), %eax
	movzbl	1(%eax), %eax
	movzbl	%al, %eax
	sall	$8, %eax
	orl	%eax, %edx
	movl	-12(%ebp), %eax
	movzbl	2(%eax), %eax
	movzbl	%al, %eax
	orl	%edx, %eax
	movl	%eax, -20(%ebp)
	jmp	L16
L9:
	movl	-24(%ebp), %eax
	movzwl	(%eax), %eax
	movzwl	%ax, %eax
	andl	$16384, %eax
	testl	%eax, %eax
	je	L11
	cmpl	$524287, 16(%ebp)
	jg	L13
	movl	16(%ebp), %edx
	leal	L_memory$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	 movl (%eax,%edx,4),%eax
	 bswap %eax
	 andl $0x00FFFFFF, %eax
	 movl %eax,-20(%ebp)
	jmp	L16
L13:
	movl	$16777215, -20(%ebp)
	jmp	L16
L11:
	movl	$10855845, -20(%ebp)
	jmp	L16
L2:
	movl	$5921370, -20(%ebp)
L16:
	movl	-20(%ebp), %eax
	addl	$20, %esp
	popl	%ebx
	leave
	ret
	.data
_nop.2447:
	.byte	0
	.byte	72
	.byte	0
	.byte	-127
_outside_executable_space.2446:
	.byte	0
	.byte	117
	.byte	0
	.byte	95
	.text
.globl _memory_read
_memory_read:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$36, %esp
	call	L38
"L00000000002$pb":
L38:
	popl	%ebx
	movl	8(%ebp), %eax
	andl	$16515072, %eax
	movl	%eax, -20(%ebp)
	cmpl	$0, -20(%ebp)
	je	L20
	movl	8(%ebp), %eax
	andl	$262143, %eax
	movl	%eax, -16(%ebp)
	sarl	$18, -20(%ebp)
	jmp	L22
L20:
	movl	8(%ebp), %eax
	andl	$258048, %eax
	movl	%eax, -20(%ebp)
	movl	8(%ebp), %eax
	andl	$4095, %eax
	movl	%eax, -16(%ebp)
	sarl	$12, -20(%ebp)
L22:
	movl	-20(%ebp), %eax
	leal	L_base$non_lazy_ptr-"L00000000002$pb"(%ebx), %edx
	movl	(%edx), %edx
	movl	(%edx,%eax,4), %eax
	movl	%eax, -12(%ebp)
	cmpl	$0, -20(%ebp)
	je	L23
	cmpl	$7, -20(%ebp)
	jg	L23
	leal	L_psr$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	-20(%ebp), %ecx
	sall	%cl, %eax
	andl	$32768, %eax
	testl	%eax, %eax
	je	L23
	movl	-20(%ebp), %eax
	orl	$64, %eax
	leal	L_base$non_lazy_ptr-"L00000000002$pb"(%ebx), %edx
	movl	(%edx), %edx
	movl	(%edx,%eax,4), %eax
	movl	%eax, -12(%ebp)
L23:
	movl	-12(%ebp), %eax
	andl	$4194304, %eax
	testl	%eax, %eax
	je	L27
	movl	-12(%ebp), %eax
	andl	$63, %eax
	testl	%eax, %eax
	je	L27
	leal	L_psr$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	je	L30
	leal	_nop.2447-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, -28(%ebp)
	jmp	L32
L30:
	leal	_outside_executable_space.2446-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, -28(%ebp)
	jmp	L32
L27:
	movl	-12(%ebp), %eax
	andl	$4194303, %eax
	sall	$12, %eax
	orl	%eax, -16(%ebp)
	cmpl	$524287, -16(%ebp)
	jbe	L33
	leal	L_psr$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	je	L35
	leal	_nop.2447-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, -28(%ebp)
	jmp	L32
L35:
	leal	_outside_executable_space.2446-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, -28(%ebp)
	jmp	L32
L33:
	movl	-16(%ebp), %eax
	leal	L_memory$non_lazy_ptr-"L00000000002$pb"(%ebx), %edx
	movl	(%edx), %edx
	movl	(%edx,%eax,4), %eax
	movl	%eax, -28(%ebp)
L32:
	movl	-28(%ebp), %eax
	addl	$36, %esp
	popl	%ebx
	leave
	ret
.globl _operand_read
_operand_read:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$68, %esp
	call	L72
"L00000000003$pb":
L72:
	popl	%ebx
	cmpl	$4, 12(%ebp)
	jne	L40
	movl	8(%ebp), %eax
	movl	%eax, -44(%ebp)
	jmp	L42
L40:
	cmpl	$5, 12(%ebp)
	jne	L43
	movl	8(%ebp), %edx
	movl	%edx, -44(%ebp)
	jmp	L42
L43:
	cmpl	$255, 8(%ebp)
	jg	L45
	movl	8(%ebp), %eax
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %edx
	movl	(%edx), %edx
	movl	(%edx,%eax,4), %eax
	movl	%eax, -44(%ebp)
	jmp	L42
L45:
	movl	8(%ebp), %eax
	andl	$16515072, %eax
	movl	%eax, -12(%ebp)
	cmpl	$0, -12(%ebp)
	je	L47
	shrl	$18, -12(%ebp)
	movl	8(%ebp), %eax
	andl	$262143, %eax
	movl	%eax, -16(%ebp)
	jmp	L49
L47:
	movl	8(%ebp), %eax
	andl	$258048, %eax
	sarl	$12, %eax
	movl	%eax, -12(%ebp)
	movl	8(%ebp), %eax
	andl	$4095, %eax
	movl	%eax, -16(%ebp)
L49:
	movl	-12(%ebp), %eax
	leal	L_base$non_lazy_ptr-"L00000000003$pb"(%ebx), %edx
	movl	(%edx), %edx
	movl	(%edx,%eax,4), %eax
	movl	%eax, -20(%ebp)
	cmpl	$0, -12(%ebp)
	je	L50
	cmpl	$7, -12(%ebp)
	ja	L50
	movl	-12(%ebp), %ecx
	movl	$32768, %eax
	movl	%eax, %edx
	sarl	%cl, %edx
	leal	L_psr$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	%edx, %eax
	testl	%eax, %eax
	je	L50
	movl	-12(%ebp), %eax
	addl	$64, %eax
	leal	L_base$non_lazy_ptr-"L00000000003$pb"(%ebx), %edx
	movl	(%edx), %edx
	movl	(%edx,%eax,4), %eax
	movl	%eax, -20(%ebp)
L50:
	movl	-20(%ebp), %eax
	andl	$12582912, %eax
	cmpl	$4194304, %eax
	jne	L54
	movl	-20(%ebp), %eax
	andl	$63, %eax
	movl	%eax, -32(%ebp)
	cmpl	$0, -32(%ebp)
	je	L54
	movl	-16(%ebp), %eax
	movl	-20(%ebp), %edx
	movl	%eax, 8(%esp)
	movl	%edx, 4(%esp)
	movl	-32(%ebp), %eax
	movl	%eax, (%esp)
	call	_device_read
	movl	%eax, -44(%ebp)
	jmp	L42
L54:
	movl	-20(%ebp), %eax
	andl	$4194303, %eax
	sall	$12, %eax
	orl	%eax, -16(%ebp)
	cmpl	$524287, -16(%ebp)
	jbe	L57
	leal	L_psr$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	jne	L59
	movl	$95, (%esp)
	call	_ii
L59:
	movl	$16775850, -44(%ebp)
	jmp	L42
L57:
	movl	-16(%ebp), %eax
	sall	$2, %eax
	movl	%eax, %edx
	leal	L_memory$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	leal	(%edx,%eax), %eax
	movl	%eax, -28(%ebp)
	movl	-28(%ebp), %eax
	 movl (%eax),%eax
	 bswap %eax
	 shll	$8, %eax
	 movl %eax,-24(%ebp)
	cmpl	$7, 12(%ebp)
	ja	L61
	movl	12(%ebp), %eax
	leal	0(,%eax,4), %edx
	leal	L66-"L00000000003$pb"(%ebx), %eax
	movl	(%edx,%eax), %eax
	addl	%ebx, %eax
	jmp	*%eax
	.align 2,0x90
L66:
	.long	L61-"L00000000003$pb"
	.long	L63-"L00000000003$pb"
	.long	L64-"L00000000003$pb"
	.long	L65-"L00000000003$pb"
	.long	L61-"L00000000003$pb"
	.long	L61-"L00000000003$pb"
	.long	L61-"L00000000003$pb"
	.long	L61-"L00000000003$pb"
L63:
	movl	-24(%ebp), %eax
	sarl	$16, %eax
	movl	%eax, -24(%ebp)
	jmp	L61
L64:
	leal	L_psr$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$128, %eax
	testl	%eax, %eax
	je	L67
	movl	-24(%ebp), %eax
	sarl	$12, %eax
	movl	%eax, -24(%ebp)
	jmp	L61
L67:
	movl	-24(%ebp), %eax
	sall	$8, %eax
	sarl	$16, %eax
	movl	%eax, -24(%ebp)
	jmp	L61
L65:
	leal	L_psr$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$128, %eax
	testl	%eax, %eax
	je	L69
	movl	-24(%ebp), %eax
	sall	$12, %eax
	sarl	$12, %eax
	movl	%eax, -24(%ebp)
	jmp	L61
L69:
	movl	-24(%ebp), %eax
	sall	$16, %eax
	sarl	$16, %eax
	movl	%eax, -24(%ebp)
L61:
	movl	-24(%ebp), %eax
	movl	%eax, %edx
	shrl	$8, %edx
	movl	%edx, -44(%ebp)
L42:
	movl	-44(%ebp), %eax
	addl	$68, %esp
	popl	%ebx
	leave
	ret
.globl _burst_read2
_burst_read2:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	pushl	%ebx
	subl	$48, %esp
	call	L92
"L00000000004$pb":
L92:
	popl	%ebx
	cmpl	$255, 12(%ebp)
	jg	L74
	movl	12(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %edx
	movl	8(%ebp), %eax
	movl	%edx, (%eax)
	movl	8(%ebp), %ecx
	addl	$4, %ecx
	movl	12(%ebp), %edx
	incl	%edx
	leal	L__register$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, (%ecx)
	jmp	L91
L74:
	movl	12(%ebp), %eax
	andl	$16515072, %eax
	movl	%eax, -12(%ebp)
	cmpl	$0, -12(%ebp)
	je	L77
	shrl	$18, -12(%ebp)
	movl	12(%ebp), %eax
	andl	$262143, %eax
	movl	%eax, -20(%ebp)
	jmp	L79
L77:
	movl	12(%ebp), %eax
	andl	$258048, %eax
	sarl	$12, %eax
	movl	%eax, -12(%ebp)
	movl	12(%ebp), %eax
	andl	$4095, %eax
	movl	%eax, -20(%ebp)
L79:
	movl	-12(%ebp), %eax
	leal	L_base$non_lazy_ptr-"L00000000004$pb"(%ebx), %edx
	movl	(%edx), %edx
	movl	(%edx,%eax,4), %eax
	movl	%eax, -16(%ebp)
	cmpl	$0, -12(%ebp)
	je	L80
	cmpl	$7, -12(%ebp)
	ja	L80
	leal	L_psr$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	-12(%ebp), %ecx
	sall	%cl, %eax
	andl	$32768, %eax
	testl	%eax, %eax
	je	L80
	movl	-12(%ebp), %eax
	addl	$64, %eax
	leal	L_base$non_lazy_ptr-"L00000000004$pb"(%ebx), %edx
	movl	(%edx), %edx
	movl	(%edx,%eax,4), %eax
	movl	%eax, -16(%ebp)
L80:
	movl	-16(%ebp), %eax
	andl	$12582912, %eax
	cmpl	$4194304, %eax
	jne	L84
	movl	-16(%ebp), %eax
	andl	$63, %eax
	movl	%eax, -24(%ebp)
	cmpl	$0, -24(%ebp)
	je	L84
	movl	-20(%ebp), %eax
	movl	-16(%ebp), %edx
	movl	-24(%ebp), %ecx
	movl	%eax, 8(%esp)
	movl	%edx, 4(%esp)
	movl	%ecx, (%esp)
	call	_device_read
	movl	%eax, %edx
	movl	8(%ebp), %eax
	movl	%edx, (%eax)
	movl	8(%ebp), %esi
	addl	$4, %esi
	movl	-20(%ebp), %eax
	incl	%eax
	movl	-16(%ebp), %edx
	movl	-24(%ebp), %ecx
	movl	%eax, 8(%esp)
	movl	%edx, 4(%esp)
	movl	%ecx, (%esp)
	call	_device_read
	movl	%eax, (%esi)
	jmp	L91
L84:
	movl	-16(%ebp), %eax
	andl	$4194303, %eax
	sall	$12, %eax
	orl	%eax, -20(%ebp)
	cmpl	$524286, -20(%ebp)
	jbe	L87
	leal	L_psr$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	jne	L91
	movl	$95, (%esp)
	call	_ii
	jmp	L91
L87:
	movl	-20(%ebp), %eax
	sall	$2, %eax
	movl	%eax, %edx
	leal	L_memory$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	leal	(%edx,%eax), %eax
	movl	%eax, -28(%ebp)
	movl	-28(%ebp), %eax
	 movl (%eax),%eax
	 bswap %eax
	 andl $0x00FFFFFF, %eax
	movl	8(%ebp), %edx
	 movl %eax,(%edx)
	movl	-28(%ebp), %eax
	addl	$4, %eax
	 movl (%eax),%eax
	 bswap %eax
	 andl $0x00FFFFFF, %eax
	movl	8(%ebp), %edx
	addl	$4, %edx
	 movl %eax,(%edx)
L91:
	addl	$48, %esp
	popl	%ebx
	popl	%esi
	leave
	ret
.globl _burst_read4
_burst_read4:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	pushl	%ebx
	subl	$48, %esp
	call	L112
"L00000000005$pb":
L112:
	popl	%ebx
	cmpl	$255, 12(%ebp)
	jg	L94
	movl	12(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %edx
	movl	8(%ebp), %eax
	movl	%edx, (%eax)
	movl	8(%ebp), %ecx
	addl	$4, %ecx
	movl	12(%ebp), %edx
	incl	%edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, (%ecx)
	movl	8(%ebp), %ecx
	addl	$8, %ecx
	movl	12(%ebp), %edx
	addl	$2, %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, (%ecx)
	movl	8(%ebp), %ecx
	addl	$12, %ecx
	movl	12(%ebp), %edx
	addl	$3, %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, (%ecx)
	jmp	L111
L94:
	movl	12(%ebp), %eax
	andl	$16515072, %eax
	movl	%eax, -16(%ebp)
	cmpl	$0, -16(%ebp)
	je	L97
	shrl	$18, -16(%ebp)
	movl	12(%ebp), %eax
	andl	$262143, %eax
	movl	%eax, -24(%ebp)
	jmp	L99
L97:
	movl	12(%ebp), %eax
	andl	$258048, %eax
	sarl	$12, %eax
	movl	%eax, -16(%ebp)
	movl	12(%ebp), %eax
	andl	$4095, %eax
	movl	%eax, -24(%ebp)
L99:
	movl	-16(%ebp), %eax
	leal	L_base$non_lazy_ptr-"L00000000005$pb"(%ebx), %edx
	movl	(%edx), %edx
	movl	(%edx,%eax,4), %eax
	movl	%eax, -20(%ebp)
	cmpl	$0, -16(%ebp)
	je	L100
	cmpl	$7, -16(%ebp)
	ja	L100
	leal	L_psr$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	-16(%ebp), %ecx
	sall	%cl, %eax
	andl	$32768, %eax
	testl	%eax, %eax
	je	L100
	movl	-16(%ebp), %eax
	addl	$64, %eax
	leal	L_base$non_lazy_ptr-"L00000000005$pb"(%ebx), %edx
	movl	(%edx), %edx
	movl	(%edx,%eax,4), %eax
	movl	%eax, -20(%ebp)
L100:
	movl	-20(%ebp), %eax
	andl	$12582912, %eax
	cmpl	$4194304, %eax
	jne	L104
	movl	-20(%ebp), %eax
	andl	$63, %eax
	movl	%eax, -12(%ebp)
	cmpl	$0, -12(%ebp)
	je	L104
	movl	-24(%ebp), %eax
	movl	-20(%ebp), %edx
	movl	-12(%ebp), %ecx
	movl	%eax, 8(%esp)
	movl	%edx, 4(%esp)
	movl	%ecx, (%esp)
	call	_device_read
	movl	%eax, %edx
	movl	8(%ebp), %eax
	movl	%edx, (%eax)
	movl	8(%ebp), %esi
	addl	$4, %esi
	movl	-24(%ebp), %eax
	incl	%eax
	movl	-20(%ebp), %edx
	movl	-12(%ebp), %ecx
	movl	%eax, 8(%esp)
	movl	%edx, 4(%esp)
	movl	%ecx, (%esp)
	call	_device_read
	movl	%eax, (%esi)
	movl	8(%ebp), %esi
	addl	$8, %esi
	movl	-24(%ebp), %eax
	addl	$2, %eax
	movl	-20(%ebp), %edx
	movl	-12(%ebp), %ecx
	movl	%eax, 8(%esp)
	movl	%edx, 4(%esp)
	movl	%ecx, (%esp)
	call	_device_read
	movl	%eax, (%esi)
	movl	8(%ebp), %esi
	addl	$12, %esi
	movl	-24(%ebp), %eax
	addl	$3, %eax
	movl	-20(%ebp), %edx
	movl	-12(%ebp), %ecx
	movl	%eax, 8(%esp)
	movl	%edx, 4(%esp)
	movl	%ecx, (%esp)
	call	_device_read
	movl	%eax, (%esi)
	jmp	L111
L104:
	movl	-20(%ebp), %eax
	andl	$4194303, %eax
	sall	$12, %eax
	orl	%eax, -24(%ebp)
	cmpl	$524284, -24(%ebp)
	jbe	L107
	leal	L_psr$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	jne	L111
	movl	$95, (%esp)
	call	_ii
	jmp	L111
L107:
	movl	-24(%ebp), %eax
	sall	$2, %eax
	movl	%eax, %edx
	leal	L_memory$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	leal	(%edx,%eax), %eax
	movl	%eax, -28(%ebp)
	movl	-28(%ebp), %eax
	 movl (%eax),%eax
	 bswap %eax
	 andl $0x00FFFFFF, %eax
	movl	8(%ebp), %edx
	 movl %eax,(%edx)
	movl	-28(%ebp), %eax
	addl	$4, %eax
	 movl (%eax),%eax
	 bswap %eax
	 andl $0x00FFFFFF, %eax
	movl	8(%ebp), %edx
	addl	$4, %edx
	 movl %eax,(%edx)
	movl	-28(%ebp), %eax
	addl	$8, %eax
	 movl (%eax),%eax
	 bswap %eax
	 andl $0x00FFFFFF, %eax
	movl	8(%ebp), %edx
	addl	$8, %edx
	 movl %eax,(%edx)
	movl	-28(%ebp), %eax
	addl	$12, %eax
	 movl (%eax),%eax
	 bswap %eax
	 andl $0x00FFFFFF, %eax
	movl	8(%ebp), %edx
	addl	$12, %edx
	 movl %eax,(%edx)
L111:
	addl	$48, %esp
	popl	%ebx
	popl	%esi
	leave
	ret
_device_write:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$20, %esp
	call	L125
"L00000000006$pb":
L125:
	popl	%ebx
	movl	12(%ebp), %eax
	sall	$3, %eax
	movl	%eax, %edx
	leal	_devices-"L00000000006$pb"(%ebx), %eax
	leal	(%edx,%eax), %eax
	movl	%eax, -20(%ebp)
	movl	-20(%ebp), %eax
	movzwl	(%eax), %eax
	testw	%ax, %ax
	jns	L124
	andl	$4194240, 16(%ebp)
	movl	16(%ebp), %eax
	sall	$12, %eax
	orl	%eax, 20(%ebp)
	movl	-20(%ebp), %eax
	movzwl	(%eax), %eax
	movzwl	%ax, %eax
	andl	$1, %eax
	testb	%al, %al
	je	L116
	cmpl	$63, 16(%ebp)
	jg	L124
	movl	-20(%ebp), %eax
	movl	4(%eax), %eax
	movl	%eax, %edx
	movl	20(%ebp), %eax
	addl	%eax, %eax
	leal	(%edx,%eax), %eax
	movl	%eax, -16(%ebp)
	movl	8(%ebp), %eax
	movl	%eax, %edx
	movl	-16(%ebp), %eax
	movb	%dl, 1(%eax)
	movl	8(%ebp), %eax
	sarl	$8, %eax
	movl	%eax, %edx
	movl	-16(%ebp), %eax
	movb	%dl, (%eax)
	jmp	L124
L116:
	movl	-20(%ebp), %eax
	movzwl	(%eax), %eax
	movzwl	%ax, %eax
	andl	$2, %eax
	testl	%eax, %eax
	je	L120
	movl	-20(%ebp), %eax
	movl	4(%eax), %eax
	movl	%eax, %ecx
	movl	20(%ebp), %edx
	movl	%edx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	leal	(%ecx,%eax), %eax
	movl	%eax, -12(%ebp)
	movl	8(%ebp), %eax
	movl	%eax, %edx
	movl	-12(%ebp), %eax
	movb	%dl, 2(%eax)
	movl	8(%ebp), %eax
	sarl	$8, %eax
	movl	%eax, %edx
	movl	-12(%ebp), %eax
	movb	%dl, 1(%eax)
	movl	8(%ebp), %eax
	sarl	$16, %eax
	movl	%eax, %edx
	movl	-12(%ebp), %eax
	movb	%dl, (%eax)
	jmp	L124
L120:
	movl	-20(%ebp), %eax
	movzwl	(%eax), %eax
	movzwl	%ax, %eax
	andl	$16384, %eax
	testl	%eax, %eax
	je	L124
	cmpl	$524287, 20(%ebp)
	jg	L124
	 movl 8(%ebp),%eax
	 andl $0x00FFFFFF, %eax
	 bswap %eax
	movl	20(%ebp), %ecx
	leal	L_memory$non_lazy_ptr-"L00000000006$pb"(%ebx), %edx
	movl	(%edx), %edx
	 movl %eax,(%edx,%ecx,4)
L124:
	addl	$20, %esp
	popl	%ebx
	leave
	ret
.globl _operand_write
_operand_write:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	pushl	%ebx
	subl	$48, %esp
	call	L157
"L00000000007$pb":
L157:
	popl	%ebx
	cmpl	$255, 12(%ebp)
	jg	L127
	leal	L_psr$non_lazy_ptr-"L00000000007$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	jne	L129
	cmpl	$127, 12(%ebp)
	jg	L156
L129:
	movl	12(%ebp), %ecx
	movl	8(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000007$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	jmp	L156
L127:
	movl	12(%ebp), %eax
	andl	$16515072, %eax
	movl	%eax, -12(%ebp)
	cmpl	$0, -12(%ebp)
	je	L132
	shrl	$18, -12(%ebp)
	movl	12(%ebp), %eax
	andl	$262143, %eax
	movl	%eax, -16(%ebp)
	jmp	L134
L132:
	movl	12(%ebp), %eax
	andl	$258048, %eax
	sarl	$12, %eax
	movl	%eax, -12(%ebp)
	movl	12(%ebp), %eax
	andl	$4095, %eax
	movl	%eax, -16(%ebp)
L134:
	movl	-12(%ebp), %eax
	leal	L_base$non_lazy_ptr-"L00000000007$pb"(%ebx), %edx
	movl	(%edx), %edx
	movl	(%edx,%eax,4), %eax
	movl	%eax, -20(%ebp)
	cmpl	$0, -12(%ebp)
	je	L135
	cmpl	$7, -12(%ebp)
	ja	L135
	leal	L_psr$non_lazy_ptr-"L00000000007$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	-12(%ebp), %ecx
	sall	%cl, %eax
	andl	$32768, %eax
	testl	%eax, %eax
	je	L135
	movl	-12(%ebp), %eax
	addl	$64, %eax
	leal	L_base$non_lazy_ptr-"L00000000007$pb"(%ebx), %edx
	movl	(%edx), %edx
	movl	(%edx,%eax,4), %eax
	movl	%eax, -20(%ebp)
L135:
	movl	-20(%ebp), %eax
	andl	$12582912, %eax
	cmpl	$4194304, %eax
	jne	L139
	movl	-20(%ebp), %eax
	andl	$63, %eax
	movl	%eax, -24(%ebp)
	cmpl	$0, -24(%ebp)
	je	L139
	movl	-16(%ebp), %eax
	movl	-20(%ebp), %edx
	movl	-24(%ebp), %ecx
	movl	8(%ebp), %esi
	movl	%eax, 12(%esp)
	movl	%edx, 8(%esp)
	movl	%ecx, 4(%esp)
	movl	%esi, (%esp)
	call	_device_write
	jmp	L156
L139:
	movl	-20(%ebp), %eax
	andl	$4194303, %eax
	sall	$12, %eax
	orl	%eax, -16(%ebp)
	cmpl	$524287, -16(%ebp)
	jbe	L142
	leal	L_psr$non_lazy_ptr-"L00000000007$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	jne	L156
	movl	$95, (%esp)
	call	_ii
	jmp	L156
L142:
	movl	-16(%ebp), %eax
	sall	$2, %eax
	movl	%eax, %edx
	leal	L_memory$non_lazy_ptr-"L00000000007$pb"(%ebx), %eax
	movl	(%eax), %eax
	leal	(%edx,%eax), %eax
	movl	%eax, -28(%ebp)
	cmpl	$7, 16(%ebp)
	ja	L156
	movl	16(%ebp), %eax
	leal	0(,%eax,4), %edx
	leal	L150-"L00000000007$pb"(%ebx), %eax
	movl	(%edx,%eax), %eax
	addl	%ebx, %eax
	jmp	*%eax
	.align 2,0x90
L150:
	.long	L146-"L00000000007$pb"
	.long	L147-"L00000000007$pb"
	.long	L148-"L00000000007$pb"
	.long	L149-"L00000000007$pb"
	.long	L156-"L00000000007$pb"
	.long	L156-"L00000000007$pb"
	.long	L156-"L00000000007$pb"
	.long	L146-"L00000000007$pb"
L146:
	 movl 8(%ebp),%eax
	 andl $0x00FFFFFF, %eax
	 bswap %eax
	movl	-28(%ebp), %edx
	 movl %eax,(%edx)
	jmp	L156
L147:
	movl	8(%ebp), %eax
	movl	%eax, %edx
	movl	-28(%ebp), %eax
	movb	%dl, 1(%eax)
	jmp	L156
L148:
	leal	L_psr$non_lazy_ptr-"L00000000007$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$128, %eax
	testl	%eax, %eax
	je	L151
	movl	-28(%ebp), %eax
	movzbl	2(%eax), %eax
	movl	%eax, %edx
	andl	$15, %edx
	movl	8(%ebp), %eax
	sall	$4, %eax
	orl	%edx, %eax
	movl	%eax, %edx
	movl	-28(%ebp), %eax
	movb	%dl, 2(%eax)
	movl	8(%ebp), %eax
	sarl	$4, %eax
	movl	%eax, %edx
	movl	-28(%ebp), %eax
	movb	%dl, 1(%eax)
	jmp	L156
L151:
	movl	8(%ebp), %eax
	movl	%eax, %edx
	movl	-28(%ebp), %eax
	movb	%dl, 2(%eax)
	jmp	L156
L149:
	leal	L_psr$non_lazy_ptr-"L00000000007$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$128, %eax
	testl	%eax, %eax
	je	L154
	movl	8(%ebp), %eax
	movl	%eax, %edx
	movl	-28(%ebp), %eax
	movb	%dl, 3(%eax)
	movl	-28(%ebp), %eax
	movzbl	2(%eax), %eax
	movl	%eax, %edx
	movl	$-16, %eax
	andl	%eax, %edx
	movl	8(%ebp), %eax
	sarl	$8, %eax
	andl	$15, %eax
	orl	%edx, %eax
	movl	%eax, %edx
	movl	-28(%ebp), %eax
	movb	%dl, 2(%eax)
	jmp	L156
L154:
	movl	8(%ebp), %eax
	movl	%eax, %edx
	movl	-28(%ebp), %eax
	movb	%dl, 3(%eax)
L156:
	addl	$48, %esp
	popl	%ebx
	popl	%esi
	leave
	ret
.globl _burst_write2
_burst_write2:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	pushl	%ebx
	subl	$48, %esp
	call	L179
"L00000000008$pb":
L179:
	popl	%ebx
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -32(%ebp)
	movl	8(%ebp), %eax
	addl	$4, %eax
	movl	(%eax), %eax
	movl	%eax, -36(%ebp)
	cmpl	$255, 12(%ebp)
	jg	L159
	leal	L_psr$non_lazy_ptr-"L00000000008$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	jne	L161
	cmpl	$126, 12(%ebp)
	jg	L178
L161:
	movl	12(%ebp), %ecx
	movl	-32(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000008$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	movl	12(%ebp), %ecx
	incl	%ecx
	movl	-36(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000008$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	jmp	L178
L159:
	movl	12(%ebp), %eax
	andl	$16515072, %eax
	movl	%eax, -16(%ebp)
	cmpl	$0, -16(%ebp)
	je	L164
	shrl	$18, -16(%ebp)
	movl	12(%ebp), %eax
	andl	$262143, %eax
	movl	%eax, -24(%ebp)
	jmp	L166
L164:
	movl	12(%ebp), %eax
	andl	$258048, %eax
	sarl	$12, %eax
	movl	%eax, -16(%ebp)
	movl	12(%ebp), %eax
	andl	$4095, %eax
	movl	%eax, -24(%ebp)
L166:
	movl	-16(%ebp), %eax
	leal	L_base$non_lazy_ptr-"L00000000008$pb"(%ebx), %edx
	movl	(%edx), %edx
	movl	(%edx,%eax,4), %eax
	movl	%eax, -20(%ebp)
	cmpl	$0, -16(%ebp)
	je	L167
	cmpl	$7, -16(%ebp)
	ja	L167
	leal	L_psr$non_lazy_ptr-"L00000000008$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	-16(%ebp), %ecx
	sall	%cl, %eax
	andl	$32768, %eax
	testl	%eax, %eax
	je	L167
	movl	-16(%ebp), %eax
	addl	$64, %eax
	leal	L_base$non_lazy_ptr-"L00000000008$pb"(%ebx), %edx
	movl	(%edx), %edx
	movl	(%edx,%eax,4), %eax
	movl	%eax, -20(%ebp)
L167:
	movl	-20(%ebp), %eax
	andl	$12582912, %eax
	cmpl	$4194304, %eax
	jne	L171
	movl	-20(%ebp), %eax
	andl	$63, %eax
	movl	%eax, -12(%ebp)
	cmpl	$0, -12(%ebp)
	je	L171
	movl	-24(%ebp), %eax
	movl	-20(%ebp), %edx
	movl	-12(%ebp), %ecx
	movl	-32(%ebp), %esi
	movl	%eax, 12(%esp)
	movl	%edx, 8(%esp)
	movl	%ecx, 4(%esp)
	movl	%esi, (%esp)
	call	_device_write
	movl	-24(%ebp), %eax
	incl	%eax
	movl	-20(%ebp), %edx
	movl	-12(%ebp), %ecx
	movl	-36(%ebp), %esi
	movl	%eax, 12(%esp)
	movl	%edx, 8(%esp)
	movl	%ecx, 4(%esp)
	movl	%esi, (%esp)
	call	_device_write
	jmp	L178
L171:
	movl	-20(%ebp), %eax
	andl	$4194303, %eax
	sall	$12, %eax
	orl	%eax, -24(%ebp)
	cmpl	$524286, -24(%ebp)
	jbe	L174
	leal	L_psr$non_lazy_ptr-"L00000000008$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	jne	L178
	movl	$95, (%esp)
	call	_ii
	jmp	L178
L174:
	movl	-24(%ebp), %eax
	sall	$2, %eax
	movl	%eax, %edx
	leal	L_memory$non_lazy_ptr-"L00000000008$pb"(%ebx), %eax
	movl	(%eax), %eax
	leal	(%edx,%eax), %eax
	movl	%eax, -28(%ebp)
	 movl -32(%ebp),%eax
	 andl $0x00FFFFFF, %eax
	 bswap %eax
	movl	-28(%ebp), %edx
	 movl %eax,(%edx)
	 movl -36(%ebp),%eax
	 andl $0x00FFFFFF, %eax
	 bswap %eax
	movl	-28(%ebp), %edx
	addl	$4, %edx
	 movl %eax,(%edx)
L178:
	addl	$48, %esp
	popl	%ebx
	popl	%esi
	leave
	ret
.globl _burst_write4
_burst_write4:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	pushl	%ebx
	subl	$64, %esp
	call	L201
"L00000000009$pb":
L201:
	popl	%ebx
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -32(%ebp)
	movl	8(%ebp), %eax
	addl	$4, %eax
	movl	(%eax), %eax
	movl	%eax, -36(%ebp)
	movl	8(%ebp), %eax
	addl	$8, %eax
	movl	(%eax), %eax
	movl	%eax, -40(%ebp)
	movl	8(%ebp), %eax
	addl	$12, %eax
	movl	(%eax), %eax
	movl	%eax, -44(%ebp)
	cmpl	$255, 12(%ebp)
	jg	L181
	leal	L_psr$non_lazy_ptr-"L00000000009$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	jne	L183
	cmpl	$124, 12(%ebp)
	jg	L200
L183:
	movl	12(%ebp), %ecx
	movl	-32(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000009$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	movl	12(%ebp), %ecx
	incl	%ecx
	movl	-36(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000009$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	movl	12(%ebp), %ecx
	addl	$2, %ecx
	movl	-40(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000009$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	movl	12(%ebp), %ecx
	addl	$3, %ecx
	movl	-44(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000009$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	jmp	L200
L181:
	movl	12(%ebp), %eax
	andl	$16515072, %eax
	movl	%eax, -16(%ebp)
	cmpl	$0, -16(%ebp)
	je	L186
	shrl	$18, -16(%ebp)
	movl	12(%ebp), %eax
	andl	$262143, %eax
	movl	%eax, -24(%ebp)
	jmp	L188
L186:
	movl	12(%ebp), %eax
	andl	$258048, %eax
	sarl	$12, %eax
	movl	%eax, -16(%ebp)
	movl	12(%ebp), %eax
	andl	$4095, %eax
	movl	%eax, -24(%ebp)
L188:
	movl	-16(%ebp), %eax
	leal	L_base$non_lazy_ptr-"L00000000009$pb"(%ebx), %edx
	movl	(%edx), %edx
	movl	(%edx,%eax,4), %eax
	movl	%eax, -20(%ebp)
	cmpl	$0, -16(%ebp)
	je	L189
	cmpl	$7, -16(%ebp)
	ja	L189
	leal	L_psr$non_lazy_ptr-"L00000000009$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	-16(%ebp), %ecx
	sall	%cl, %eax
	andl	$32768, %eax
	testl	%eax, %eax
	je	L189
	movl	-16(%ebp), %eax
	addl	$64, %eax
	leal	L_base$non_lazy_ptr-"L00000000009$pb"(%ebx), %edx
	movl	(%edx), %edx
	movl	(%edx,%eax,4), %eax
	movl	%eax, -20(%ebp)
L189:
	movl	-20(%ebp), %eax
	andl	$12582912, %eax
	cmpl	$4194304, %eax
	jne	L193
	movl	-20(%ebp), %eax
	andl	$63, %eax
	movl	%eax, -12(%ebp)
	cmpl	$0, -12(%ebp)
	je	L193
	movl	-24(%ebp), %eax
	movl	-20(%ebp), %edx
	movl	-12(%ebp), %ecx
	movl	-32(%ebp), %esi
	movl	%eax, 12(%esp)
	movl	%edx, 8(%esp)
	movl	%ecx, 4(%esp)
	movl	%esi, (%esp)
	call	_device_write
	movl	-24(%ebp), %eax
	incl	%eax
	movl	-20(%ebp), %edx
	movl	-12(%ebp), %ecx
	movl	-36(%ebp), %esi
	movl	%eax, 12(%esp)
	movl	%edx, 8(%esp)
	movl	%ecx, 4(%esp)
	movl	%esi, (%esp)
	call	_device_write
	movl	-24(%ebp), %eax
	addl	$2, %eax
	movl	-20(%ebp), %edx
	movl	-12(%ebp), %ecx
	movl	-40(%ebp), %esi
	movl	%eax, 12(%esp)
	movl	%edx, 8(%esp)
	movl	%ecx, 4(%esp)
	movl	%esi, (%esp)
	call	_device_write
	movl	-24(%ebp), %eax
	addl	$3, %eax
	movl	-20(%ebp), %edx
	movl	-12(%ebp), %ecx
	movl	-44(%ebp), %esi
	movl	%eax, 12(%esp)
	movl	%edx, 8(%esp)
	movl	%ecx, 4(%esp)
	movl	%esi, (%esp)
	call	_device_write
	jmp	L200
L193:
	movl	-20(%ebp), %eax
	andl	$4194303, %eax
	sall	$12, %eax
	orl	%eax, -24(%ebp)
	cmpl	$524284, -24(%ebp)
	jbe	L196
	leal	L_psr$non_lazy_ptr-"L00000000009$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	jne	L200
	movl	$95, (%esp)
	call	_ii
	jmp	L200
L196:
	movl	-24(%ebp), %eax
	sall	$2, %eax
	movl	%eax, %edx
	leal	L_memory$non_lazy_ptr-"L00000000009$pb"(%ebx), %eax
	movl	(%eax), %eax
	leal	(%edx,%eax), %eax
	movl	%eax, -28(%ebp)
	 movl -32(%ebp),%eax
	 andl $0x00FFFFFF, %eax
	 bswap %eax
	movl	-28(%ebp), %edx
	 movl %eax,(%edx)
	 movl -36(%ebp),%eax
	 andl $0x00FFFFFF, %eax
	 bswap %eax
	movl	-28(%ebp), %edx
	addl	$4, %edx
	 movl %eax,(%edx)
	 movl -40(%ebp),%eax
	 andl $0x00FFFFFF, %eax
	 bswap %eax
	movl	-28(%ebp), %edx
	addl	$8, %edx
	 movl %eax,(%edx)
	 movl -44(%ebp),%eax
	 andl $0x00FFFFFF, %eax
	 bswap %eax
	movl	-28(%ebp), %edx
	addl	$12, %edx
	 movl %eax,(%edx)
L200:
	addl	$64, %esp
	popl	%ebx
	popl	%esi
	leave
	ret
	.section __IMPORT,__pointers,non_lazy_symbol_pointers
L_memory$non_lazy_ptr:
	.indirect_symbol _memory
	.long	0
L__register$non_lazy_ptr:
	.indirect_symbol __register
	.long	0
L_psr$non_lazy_ptr:
	.indirect_symbol _psr
	.long	0
L_base$non_lazy_ptr:
	.indirect_symbol _base
	.long	0
	.subsections_via_symbols
