	.text
_sright:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	movl	8(%ebp), %eax
	movl	%eax, -12(%ebp)
	movl	$0, -16(%ebp)
	jmp	L2
L3:
	andl	$1, -12(%ebp)
	sall	$24, -12(%ebp)
	movl	-16(%ebp), %eax
	sall	$2, %eax
	addl	16(%ebp), %eax
	movl	(%eax), %eax
	orl	%eax, -12(%ebp)
	movl	-16(%ebp), %eax
	sall	$2, %eax
	movl	%eax, %edx
	addl	16(%ebp), %edx
	movl	-12(%ebp), %eax
	sarl	%eax
	movl	%eax, (%edx)
	incl	-16(%ebp)
L2:
	movl	-16(%ebp), %eax
	cmpl	12(%ebp), %eax
	jl	L3
	leave
	ret
_sleft:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	movl	8(%ebp), %eax
	andl	$1, %eax
	movl	%eax, -12(%ebp)
	movl	12(%ebp), %eax
	movl	%eax, -16(%ebp)
	jmp	L7
L8:
	movl	-16(%ebp), %eax
	sall	$2, %eax
	addl	16(%ebp), %eax
	movl	(%eax), %eax
	addl	%eax, %eax
	orl	%eax, -12(%ebp)
	movl	-16(%ebp), %eax
	sall	$2, %eax
	movl	%eax, %edx
	addl	16(%ebp), %edx
	movl	-12(%ebp), %eax
	andl	$16777215, %eax
	movl	%eax, (%edx)
	sarl	$24, -12(%ebp)
L7:
	decl	-16(%ebp)
	cmpl	$-1, -16(%ebp)
	jne	L8
	leave
	ret
_add:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	movl	$0, -12(%ebp)
	movl	8(%ebp), %eax
	movl	%eax, -16(%ebp)
	jmp	L12
L13:
	movl	-16(%ebp), %eax
	sall	$2, %eax
	addl	12(%ebp), %eax
	movl	(%eax), %eax
	addl	%eax, -12(%ebp)
	movl	-16(%ebp), %eax
	sall	$2, %eax
	addl	16(%ebp), %eax
	movl	(%eax), %eax
	addl	%eax, -12(%ebp)
	movl	-16(%ebp), %eax
	sall	$2, %eax
	movl	%eax, %edx
	addl	12(%ebp), %edx
	movl	-12(%ebp), %eax
	andl	$16777215, %eax
	movl	%eax, (%edx)
	shrl	$24, -12(%ebp)
L12:
	decl	-16(%ebp)
	cmpl	$-1, -16(%ebp)
	jne	L13
	movl	-12(%ebp), %eax
	leave
	ret
_addcarry:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	movl	8(%ebp), %eax
	movl	%eax, -12(%ebp)
	movl	12(%ebp), %eax
	movl	%eax, -16(%ebp)
	jmp	L17
L18:
	movl	-16(%ebp), %eax
	sall	$2, %eax
	addl	16(%ebp), %eax
	movl	(%eax), %eax
	addl	%eax, -12(%ebp)
	movl	-16(%ebp), %eax
	sall	$2, %eax
	addl	20(%ebp), %eax
	movl	(%eax), %eax
	addl	%eax, -12(%ebp)
	movl	-16(%ebp), %eax
	sall	$2, %eax
	movl	%eax, %edx
	addl	24(%ebp), %edx
	movl	-12(%ebp), %eax
	andl	$16777215, %eax
	movl	%eax, (%edx)
	sarl	$24, -12(%ebp)
L17:
	decl	-16(%ebp)
	cmpl	$-1, -16(%ebp)
	jne	L18
	movl	-12(%ebp), %eax
	leave
	ret
_store_minor_result:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$36, %esp
	call	L30
"L00000000001$pb":
L30:
	popl	%ebx
	movl	$0, -12(%ebp)
	subl	$72, 12(%ebp)
	jmp	L22
L23:
	movl	16(%ebp), %eax
	movl	(%eax), %eax
	xorl	8(%ebp), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	jne	L24
	movl	16(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	$3, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_sleft
	incl	-12(%ebp)
L22:
	cmpl	$71, -12(%ebp)
	jle	L23
L24:
	cmpl	$71, -12(%ebp)
	jg	L26
	movl	-12(%ebp), %eax
	subl	%eax, 12(%ebp)
	jmp	L28
L26:
	movl	$0, 12(%ebp)
L28:
	andl	$8388607, 12(%ebp)
	leal	L_iselect$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$8, %ecx
	movl	8(%ebp), %eax
	movl	12(%ebp), %edx
	xorl	%eax, %edx
	leal	L__register$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$9, %ecx
	movl	16(%ebp), %eax
	movl	(%eax), %edx
	leal	L__register$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$10, %ecx
	movl	16(%ebp), %eax
	addl	$4, %eax
	movl	(%eax), %edx
	leal	L__register$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$11, %ecx
	movl	16(%ebp), %eax
	addl	$8, %eax
	movl	(%eax), %edx
	leal	L__register$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	addl	$36, %esp
	popl	%ebx
	leave
	ret
_add_bias:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$132, %esp
	call	L58
"L00000000002$pb":
L58:
	popl	%ebx
	movl	$0, -12(%ebp)
	movl	16(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -16(%ebp)
	movl	8(%ebp), %ecx
	movl	$715827883, -108(%ebp)
	movl	-108(%ebp), %eax
	imull	%ecx
	sarl	$2, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	movl	%edx, %ecx
	subl	%eax, %ecx
	movl	%ecx, %eax
	incl	%eax
	movl	%eax, -20(%ebp)
	movl	8(%ebp), %ecx
	movl	$715827883, -108(%ebp)
	movl	-108(%ebp), %eax
	imull	%ecx
	sarl	$2, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movl	%ecx, %edx
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, -24(%ebp)
	movl	$24, %eax
	subl	-24(%ebp), %eax
	movl	%eax, -28(%ebp)
	movl	16(%ebp), %eax
	addl	$4, %eax
	movl	(%eax), %eax
	movl	%eax, -32(%ebp)
	movl	12(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -36(%ebp)
	movl	$0, -40(%ebp)
	movl	$2, -44(%ebp)
	movl	$3, -48(%ebp)
	movl	$3, -52(%ebp)
	movl	$-71, -56(%ebp)
	leal	L_psr$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$8, %eax
	testl	%eax, %eax
	je	L32
	movl	$6, -48(%ebp)
	movl	12(%ebp), %edx
	addl	$28, %edx
	movl	12(%ebp), %eax
	addl	$16, %eax
	movl	(%eax), %eax
	movl	%eax, (%edx)
	movl	12(%ebp), %edx
	addl	$24, %edx
	movl	-36(%ebp), %eax
	movl	%eax, (%edx)
	movl	12(%ebp), %edx
	addl	$20, %edx
	movl	-36(%ebp), %eax
	movl	%eax, (%edx)
	movl	12(%ebp), %edx
	addl	$16, %edx
	movl	-36(%ebp), %eax
	movl	%eax, (%edx)
	movl	16(%ebp), %edx
	addl	$28, %edx
	movl	-16(%ebp), %eax
	movl	%eax, (%edx)
	movl	16(%ebp), %edx
	addl	$24, %edx
	movl	-16(%ebp), %eax
	movl	%eax, (%edx)
	movl	16(%ebp), %edx
	addl	$20, %edx
	movl	-16(%ebp), %eax
	movl	%eax, (%edx)
	movl	$6, -52(%ebp)
	movl	$-143, -56(%ebp)
	cmpl	$160, 8(%ebp)
	jle	L39
	movl	$0, -92(%ebp)
	jmp	L36
L32:
	cmpl	$80, 8(%ebp)
	jle	L39
	movl	$0, -92(%ebp)
	jmp	L36
L40:
	movl	-40(%ebp), %edx
	movl	-16(%ebp), %eax
	movl	%eax, -88(%ebp,%edx,4)
	incl	-40(%ebp)
L39:
	movl	-40(%ebp), %eax
	cmpl	-20(%ebp), %eax
	jl	L40
L41:
	movl	-28(%ebp), %ecx
	sall	%cl, -16(%ebp)
	movl	-24(%ebp), %ecx
	movl	-32(%ebp), %eax
	sarl	%cl, %eax
	orl	%eax, -16(%ebp)
	andl	$16777215, -16(%ebp)
	movl	-40(%ebp), %edx
	movl	-16(%ebp), %eax
	movl	%eax, -88(%ebp,%edx,4)
	movl	-40(%ebp), %eax
	cmpl	-48(%ebp), %eax
	jg	L42
	incl	-40(%ebp)
	movl	-32(%ebp), %eax
	movl	%eax, -16(%ebp)
	movl	-44(%ebp), %eax
	sall	$2, %eax
	addl	16(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -32(%ebp)
	incl	-44(%ebp)
	jmp	L41
L42:
	movl	-40(%ebp), %eax
	sall	$2, %eax
	addl	12(%ebp), %eax
	movl	(%eax), %eax
	addl	%eax, -16(%ebp)
	movl	-40(%ebp), %eax
	sall	$2, %eax
	movl	%eax, %edx
	addl	12(%ebp), %edx
	movl	-16(%ebp), %eax
	andl	$16777215, %eax
	movl	%eax, (%edx)
	decl	-40(%ebp)
	cmpl	$0, -40(%ebp)
	js	L44
	sarl	$24, -16(%ebp)
	movl	-40(%ebp), %eax
	movl	-88(%ebp,%eax,4), %eax
	addl	%eax, -16(%ebp)
	jmp	L42
L44:
	movl	12(%ebp), %eax
	movl	(%eax), %eax
	xorl	-36(%ebp), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	je	L46
	xorl	$16777215, -36(%ebp)
	movl	-36(%ebp), %eax
	movl	%eax, -16(%ebp)
	cmpl	$0, -16(%ebp)
	jne	L50
	movl	$1, -16(%ebp)
	jmp	L50
L51:
	movl	-52(%ebp), %eax
	sall	$2, %eax
	addl	12(%ebp), %eax
	movl	(%eax), %eax
	addl	%eax, -16(%ebp)
	movl	-52(%ebp), %eax
	sall	$2, %eax
	movl	%eax, %edx
	addl	12(%ebp), %edx
	movl	-16(%ebp), %eax
	andl	$16777215, %eax
	movl	%eax, (%edx)
	sarl	$24, -16(%ebp)
	decl	-52(%ebp)
	movl	-36(%ebp), %eax
	addl	%eax, -16(%ebp)
L50:
	cmpl	$0, -52(%ebp)
	jne	L51
L46:
	movl	12(%ebp), %eax
	movl	(%eax), %eax
	xorl	-36(%ebp), %eax
	andl	$1, %eax
	testb	%al, %al
	je	L55
	movl	-48(%ebp), %eax
	incl	%eax
	movl	12(%ebp), %edx
	movl	%edx, 8(%esp)
	movl	%eax, 4(%esp)
	movl	-36(%ebp), %eax
	movl	%eax, (%esp)
	call	_sright
	movl	$1, -12(%ebp)
	jmp	L54
L56:
	movl	-48(%ebp), %edx
	incl	%edx
	movl	12(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	%edx, 4(%esp)
	movl	-36(%ebp), %eax
	movl	%eax, (%esp)
	call	_sleft
	decl	-12(%ebp)
	movl	-12(%ebp), %eax
	cmpl	-56(%ebp), %eax
	jl	L54
L55:
	movl	12(%ebp), %eax
	addl	$4, %eax
	movl	(%eax), %eax
	xorl	-36(%ebp), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	je	L56
L54:
	movl	12(%ebp), %edx
	movl	-36(%ebp), %eax
	movl	%eax, (%edx)
	movl	-12(%ebp), %ecx
	movl	%ecx, -92(%ebp)
L36:
	movl	-92(%ebp), %eax
	addl	$132, %esp
	popl	%ebx
	leave
	ret
_ones_add:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$116, %esp
	call	L76
"L00000000003$pb":
L76:
	popl	%ebx
	movl	8(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	-92(%ebp), %eax
	movl	%eax, (%esp)
	call	_burst_read4
	movl	-92(%ebp), %eax
	xorl	12(%ebp), %eax
	movl	%eax, -92(%ebp)
	movl	-88(%ebp), %eax
	xorl	12(%ebp), %eax
	movl	%eax, -88(%ebp)
	movl	-84(%ebp), %eax
	xorl	12(%ebp), %eax
	movl	%eax, -84(%ebp)
	movl	-80(%ebp), %eax
	xorl	12(%ebp), %eax
	movl	%eax, -80(%ebp)
	leal	L_iselect$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$4, %edx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	sarl	$23, %eax
	andl	$1, %eax
	negl	%eax
	andl	$16777215, %eax
	movl	%eax, -16(%ebp)
	movl	-92(%ebp), %eax
	sarl	$23, %eax
	andl	$1, %eax
	negl	%eax
	andl	$16777215, %eax
	movl	%eax, -20(%ebp)
	movl	-88(%ebp), %eax
	xorl	-20(%ebp), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	je	L60
	leal	L_iselect$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$5, %edx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	xorl	-16(%ebp), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	je	L62
	leal	L_iselect$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$4, %edx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	andl	$16777215, %eax
	movl	%eax, %edx
	xorl	-16(%ebp), %edx
	movl	-92(%ebp), %eax
	andl	$16777215, %eax
	xorl	-20(%ebp), %eax
	movl	%edx, %ecx
	subl	%eax, %ecx
	movl	%ecx, %eax
	movl	%eax, -12(%ebp)
	cmpl	$0, -12(%ebp)
	jns	L64
	movl	-92(%ebp), %eax
	movl	%eax, -60(%ebp)
	movl	-88(%ebp), %eax
	movl	%eax, -56(%ebp)
	movl	-84(%ebp), %eax
	movl	%eax, -52(%ebp)
	movl	-80(%ebp), %eax
	movl	%eax, -48(%ebp)
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	588(%eax), %eax
	xorl	-20(%ebp), %eax
	movl	%eax, -44(%ebp)
	movl	-16(%ebp), %eax
	movl	%eax, -92(%ebp)
	leal	L_iselect$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$5, %edx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, -88(%ebp)
	leal	L_iselect$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$6, %edx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, -84(%ebp)
	leal	L_iselect$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$7, %edx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, -80(%ebp)
	movl	-16(%ebp), %eax
	movl	%eax, -76(%ebp)
	negl	-12(%ebp)
	movl	-20(%ebp), %eax
	movl	%eax, -16(%ebp)
	jmp	L66
L64:
	leal	L_iselect$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$4, %edx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, -60(%ebp)
	leal	L_iselect$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$5, %edx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, -56(%ebp)
	leal	L_iselect$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$6, %edx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, -52(%ebp)
	leal	L_iselect$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$7, %edx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, -48(%ebp)
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	588(%eax), %eax
	xorl	-16(%ebp), %eax
	movl	%eax, -44(%ebp)
	movl	-20(%ebp), %eax
	movl	%eax, -76(%ebp)
	movl	-20(%ebp), %eax
	movl	%eax, -92(%ebp)
L66:
	movl	-60(%ebp), %eax
	xorl	-16(%ebp), %eax
	movl	%eax, -24(%ebp)
	movl	-16(%ebp), %eax
	movl	%eax, -60(%ebp)
	leal	-92(%ebp), %eax
	movl	%eax, 8(%esp)
	leal	-60(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	-12(%ebp), %eax
	movl	%eax, (%esp)
	call	_add_bias
	movl	%eax, -28(%ebp)
	movl	-28(%ebp), %eax
	addl	%eax, -24(%ebp)
	cmpl	$-71, -28(%ebp)
	jge	L67
	movl	$4194304, -24(%ebp)
L67:
	leal	L_iselect$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$4, %ecx
	movl	-60(%ebp), %eax
	movl	%eax, %edx
	xorl	-24(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$5, %ecx
	movl	-56(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$6, %ecx
	movl	-52(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$7, %ecx
	movl	-48(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	leal	L_psr$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$8, %eax
	testl	%eax, %eax
	je	L75
	leal	-60(%ebp), %eax
	addl	$16, %eax
	movl	-60(%ebp), %edx
	movl	%eax, 8(%esp)
	movl	-24(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	_store_minor_result
	jmp	L75
L62:
	leal	L_iselect$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$4, %ecx
	movl	-92(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$5, %ecx
	movl	-88(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$6, %ecx
	movl	-84(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$7, %ecx
	movl	-80(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	leal	L_psr$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$8, %eax
	testl	%eax, %eax
	je	L75
	leal	L_iselect$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$8, %ecx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	-20(%ebp), %edx
	movl	%edx, (%eax,%ecx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$9, %ecx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	-20(%ebp), %edx
	movl	%edx, (%eax,%ecx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$10, %ecx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	-20(%ebp), %edx
	movl	%edx, (%eax,%ecx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$11, %ecx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	-20(%ebp), %edx
	movl	%edx, (%eax,%ecx,4)
	jmp	L75
L60:
	leal	L_psr$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$8, %eax
	testl	%eax, %eax
	je	L75
	leal	L_iselect$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$8, %ecx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	-16(%ebp), %edx
	movl	%edx, (%eax,%ecx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$9, %ecx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	-16(%ebp), %edx
	movl	%edx, (%eax,%ecx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$10, %ecx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	-16(%ebp), %edx
	movl	%edx, (%eax,%ecx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$11, %ecx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	-16(%ebp), %edx
	movl	%edx, (%eax,%ecx,4)
L75:
	addl	$116, %esp
	popl	%ebx
	leave
	ret
.globl _fa
_fa:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	movl	$0, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_ones_add
	leave
	ret
.globl _fan
_fan:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	movl	$16777215, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_ones_add
	leave
	ret
.globl _fm
_fm:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$188, %esp
	call	L114
"L00000000004$pb":
L114:
	popl	%ebx
	leal	L__register$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	588(%eax), %edx
	leal	L__register$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	588(%eax), %eax
	movl	$0, -96(%ebp)
	movl	$0, -92(%ebp)
	movl	$0, -88(%ebp)
	movl	%edx, -84(%ebp)
	movl	$0, -80(%ebp)
	movl	$0, -76(%ebp)
	movl	$0, -72(%ebp)
	movl	%eax, -68(%ebp)
	leal	-164(%ebp), %edi
	cld
	movl	$0, %edx
	movl	$9, %eax
	movl	%eax, %ecx
	movl	%edx, %eax
	rep
	stosl
	leal	L_iselect$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$4, %edx
	leal	L__register$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %esi
	leal	L_iselect$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$5, %edx
	leal	L__register$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %edi
	leal	L_iselect$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$6, %edx
	leal	L__register$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %ecx
	leal	L_iselect$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$7, %edx
	leal	L__register$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%esi, -128(%ebp)
	movl	%edi, -124(%ebp)
	movl	%ecx, -120(%ebp)
	movl	%eax, -116(%ebp)
	movl	$0, -112(%ebp)
	movl	$0, -108(%ebp)
	movl	$0, -104(%ebp)
	movl	$0, -100(%ebp)
	movl	$0, -64(%ebp)
	movl	$0, -60(%ebp)
	movl	$0, -56(%ebp)
	movl	$0, -52(%ebp)
	leal	L_psr$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$8, %eax
	testl	%eax, %eax
	je	L82
	movl	$7, -172(%ebp)
	jmp	L84
L82:
	movl	$3, -172(%ebp)
L84:
	movl	-172(%ebp), %eax
	movl	%eax, -48(%ebp)
	movl	8(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	-64(%ebp), %eax
	movl	%eax, (%esp)
	call	_burst_read4
	movl	-128(%ebp), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	je	L85
	movl	-164(%ebp), %eax
	xorl	$16777215, %eax
	movl	%eax, -164(%ebp)
	movl	-128(%ebp), %eax
	xorl	$16777215, %eax
	movl	%eax, -128(%ebp)
	movl	-124(%ebp), %eax
	xorl	$16777215, %eax
	movl	%eax, -124(%ebp)
	movl	-120(%ebp), %eax
	xorl	$16777215, %eax
	movl	%eax, -120(%ebp)
	movl	-116(%ebp), %eax
	xorl	$16777215, %eax
	movl	%eax, -116(%ebp)
L85:
	movl	-64(%ebp), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	je	L87
	movl	-164(%ebp), %eax
	xorl	$16777215, %eax
	movl	%eax, -164(%ebp)
	movl	-64(%ebp), %eax
	xorl	$16777215, %eax
	movl	%eax, -64(%ebp)
	movl	-60(%ebp), %eax
	xorl	$16777215, %eax
	movl	%eax, -60(%ebp)
	movl	-56(%ebp), %eax
	xorl	$16777215, %eax
	movl	%eax, -56(%ebp)
	movl	-52(%ebp), %eax
	xorl	$16777215, %eax
	movl	%eax, -52(%ebp)
L87:
	movl	-164(%ebp), %eax
	movl	%eax, -44(%ebp)
	movl	$0, -40(%ebp)
	movl	-124(%ebp), %edx
	movl	-60(%ebp), %eax
	andl	%edx, %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	je	L89
	movl	$1, -28(%ebp)
	jmp	L91
L92:
	movl	-28(%ebp), %eax
	movl	-64(%ebp,%eax,4), %eax
	movl	%eax, -36(%ebp)
	movl	$24, -32(%ebp)
	jmp	L93
L94:
	leal	-128(%ebp), %eax
	addl	$4, %eax
	movl	%eax, 8(%esp)
	movl	$7, 4(%esp)
	movl	$0, (%esp)
	call	_sright
	movl	-36(%ebp), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	je	L95
	leal	-128(%ebp), %eax
	leal	4(%eax), %edx
	leal	-164(%ebp), %eax
	addl	$4, %eax
	movl	%edx, 8(%esp)
	movl	%eax, 4(%esp)
	movl	$7, (%esp)
	call	_add
L95:
	sall	-36(%ebp)
L93:
	decl	-32(%ebp)
	cmpl	$-1, -32(%ebp)
	jne	L94
	incl	-28(%ebp)
L91:
	cmpl	$3, -28(%ebp)
	jle	L92
	movl	-128(%ebp), %eax
	movl	%eax, %edx
	andl	$8388607, %edx
	movl	-64(%ebp), %eax
	andl	$8388607, %eax
	leal	(%edx,%eax), %eax
	subl	$4194304, %eax
	movl	%eax, -40(%ebp)
	movl	-160(%ebp), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	jne	L99
	leal	-164(%ebp), %eax
	addl	$4, %eax
	movl	%eax, 8(%esp)
	movl	-48(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	$0, (%esp)
	call	_sleft
	decl	-40(%ebp)
L99:
	andl	$8388607, -40(%ebp)
	leal	L_psr$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$8, %eax
	testl	%eax, %eax
	je	L101
	movl	$0, -84(%ebp)
L101:
	leal	-164(%ebp), %eax
	leal	4(%eax), %edx
	movl	-48(%ebp), %ecx
	incl	%ecx
	leal	-96(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	%edx, 4(%esp)
	movl	%ecx, (%esp)
	call	_add
	movl	%eax, -36(%ebp)
	movl	-36(%ebp), %eax
	addl	%eax, -40(%ebp)
	cmpl	$0, -36(%ebp)
	je	L103
	leal	-164(%ebp), %eax
	addl	$4, %eax
	movl	%eax, 8(%esp)
	movl	-48(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	-36(%ebp), %eax
	movl	%eax, (%esp)
	call	_sright
L103:
	movl	-40(%ebp), %eax
	andl	$-8388608, %eax
	testl	%eax, %eax
	je	L105
	movl	$8388607, -40(%ebp)
L105:
	movl	-160(%ebp), %edx
	movl	-164(%ebp), %eax
	xorl	%edx, %eax
	movl	%eax, -160(%ebp)
	movl	-156(%ebp), %edx
	movl	-164(%ebp), %eax
	xorl	%edx, %eax
	movl	%eax, -156(%ebp)
	movl	-152(%ebp), %edx
	movl	-164(%ebp), %eax
	xorl	%edx, %eax
	movl	%eax, -152(%ebp)
	leal	L_psr$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$8, %eax
	testl	%eax, %eax
	je	L107
	movl	-164(%ebp), %eax
	movl	%eax, -44(%ebp)
	movl	-148(%ebp), %edx
	movl	-164(%ebp), %eax
	xorl	%edx, %eax
	movl	%eax, -148(%ebp)
	movl	-144(%ebp), %edx
	movl	-164(%ebp), %eax
	xorl	%edx, %eax
	movl	%eax, -144(%ebp)
	movl	-140(%ebp), %edx
	movl	-164(%ebp), %eax
	xorl	%edx, %eax
	movl	%eax, -140(%ebp)
L107:
	movl	-164(%ebp), %eax
	xorl	-40(%ebp), %eax
	movl	%eax, -164(%ebp)
	jmp	L109
L89:
	movl	-44(%ebp), %eax
	movl	%eax, -160(%ebp)
	movl	-44(%ebp), %eax
	movl	%eax, -156(%ebp)
	movl	-44(%ebp), %eax
	movl	%eax, -152(%ebp)
	leal	L_psr$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$8, %eax
	testl	%eax, %eax
	je	L109
	movl	-44(%ebp), %eax
	movl	%eax, -148(%ebp)
	movl	-44(%ebp), %eax
	movl	%eax, -144(%ebp)
	movl	-44(%ebp), %eax
	movl	%eax, -140(%ebp)
L109:
	leal	L_iselect$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$4, %ecx
	movl	-164(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$5, %ecx
	movl	-160(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$6, %ecx
	movl	-156(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$7, %ecx
	movl	-152(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	leal	L_psr$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$8, %eax
	testl	%eax, %eax
	je	L113
	leal	-164(%ebp), %eax
	addl	$16, %eax
	movl	%eax, 8(%esp)
	movl	-40(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	-44(%ebp), %eax
	movl	%eax, (%esp)
	call	_store_minor_result
L113:
	addl	$188, %esp
	popl	%ebx
	popl	%esi
	popl	%edi
	leave
	ret
.globl _fd
_fd:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$204, %esp
	call	L142
"L00000000005$pb":
L142:
	popl	%ebx
	leal	L_iselect$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$4, %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %esi
	leal	L_iselect$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$5, %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %edi
	leal	L_iselect$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$6, %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %edx
	movl	%edx, -172(%ebp)
	leal	L_iselect$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$7, %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	588(%eax), %ecx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	588(%eax), %eax
	movl	%esi, -104(%ebp)
	movl	%edi, -100(%ebp)
	movl	-172(%ebp), %esi
	movl	%esi, -96(%ebp)
	movl	%edx, -92(%ebp)
	movl	%ecx, -88(%ebp)
	movl	$0, -84(%ebp)
	movl	$0, -80(%ebp)
	movl	%eax, -76(%ebp)
	movl	$0, -136(%ebp)
	movl	$0, -132(%ebp)
	movl	$0, -128(%ebp)
	movl	$0, -124(%ebp)
	movl	$16777215, -120(%ebp)
	movl	$16777215, -116(%ebp)
	movl	$16777215, -112(%ebp)
	movl	$16777215, -108(%ebp)
	leal	-168(%ebp), %edi
	cld
	movl	$0, %edx
	movl	$8, %eax
	movl	%eax, %ecx
	movl	%edx, %eax
	rep
	stosl
	movl	$0, -28(%ebp)
	movl	$0, -32(%ebp)
	movl	$72, -36(%ebp)
	movl	$3, -40(%ebp)
	movl	8(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	-136(%ebp), %eax
	movl	%eax, (%esp)
	call	_burst_read4
	movl	-136(%ebp), %edx
	movl	-132(%ebp), %eax
	xorl	%edx, %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	jne	L116
	leal	L_iselect$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$4, %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	$16777215, (%eax,%edx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$5, %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	$16777215, (%eax,%edx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$6, %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	$16777215, (%eax,%edx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$7, %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	$16777215, (%eax,%edx,4)
	leal	L_psr$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$8, %eax
	testl	%eax, %eax
	je	L141
	leal	L_iselect$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$8, %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	$16777215, (%eax,%edx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$9, %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	$16777215, (%eax,%edx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$10, %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	$16777215, (%eax,%edx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$11, %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	$16777215, (%eax,%edx,4)
	jmp	L141
L116:
	movl	-136(%ebp), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	je	L121
	movl	$16777215, -28(%ebp)
	jmp	L123
L121:
	movl	-132(%ebp), %eax
	xorl	$16777215, %eax
	movl	%eax, -132(%ebp)
	movl	-128(%ebp), %eax
	xorl	$16777215, %eax
	movl	%eax, -128(%ebp)
	movl	-124(%ebp), %eax
	xorl	$16777215, %eax
	movl	%eax, -124(%ebp)
L123:
	movl	-136(%ebp), %eax
	xorl	-28(%ebp), %eax
	movl	%eax, -136(%ebp)
	movl	-104(%ebp), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	je	L124
	movl	$16777215, -32(%ebp)
L124:
	movl	-104(%ebp), %eax
	xorl	-32(%ebp), %eax
	movl	%eax, -104(%ebp)
	movl	-100(%ebp), %eax
	xorl	-32(%ebp), %eax
	movl	%eax, -100(%ebp)
	movl	-96(%ebp), %eax
	xorl	-32(%ebp), %eax
	movl	%eax, -96(%ebp)
	movl	-92(%ebp), %eax
	xorl	-32(%ebp), %eax
	movl	%eax, -92(%ebp)
	movl	-28(%ebp), %eax
	xorl	%eax, -32(%ebp)
	leal	L_psr$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$8, %eax
	testl	%eax, %eax
	je	L128
	movl	$144, -36(%ebp)
	movl	$0, -88(%ebp)
	movl	$6, -40(%ebp)
	jmp	L128
L129:
	leal	-136(%ebp), %eax
	leal	4(%eax), %edx
	leal	-104(%ebp), %eax
	leal	4(%eax), %ecx
	leal	-72(%ebp), %eax
	movl	%eax, 16(%esp)
	movl	%edx, 12(%esp)
	movl	%ecx, 8(%esp)
	movl	$7, 4(%esp)
	movl	$1, (%esp)
	call	_addcarry
	movl	%eax, -44(%ebp)
	cmpl	$0, -44(%ebp)
	je	L130
	movl	-72(%ebp), %eax
	movl	%eax, -100(%ebp)
	movl	-68(%ebp), %eax
	movl	%eax, -96(%ebp)
	movl	-64(%ebp), %eax
	movl	%eax, -92(%ebp)
	leal	L_psr$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$8, %eax
	testl	%eax, %eax
	je	L130
	movl	-60(%ebp), %eax
	movl	%eax, -88(%ebp)
	movl	-56(%ebp), %eax
	movl	%eax, -84(%ebp)
	movl	-52(%ebp), %eax
	movl	%eax, -80(%ebp)
L130:
	leal	-168(%ebp), %eax
	addl	$4, %eax
	movl	%eax, 8(%esp)
	movl	-40(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	-44(%ebp), %eax
	movl	%eax, (%esp)
	call	_sleft
	leal	-136(%ebp), %eax
	addl	$4, %eax
	movl	%eax, 8(%esp)
	movl	$7, 4(%esp)
	movl	$1, (%esp)
	call	_sright
L128:
	decl	-36(%ebp)
	cmpl	$-1, -36(%ebp)
	jne	L129
	movl	-104(%ebp), %edx
	movl	-136(%ebp), %eax
	movl	%edx, %ecx
	subl	%eax, %ecx
	movl	%ecx, %eax
	addl	$4194305, %eax
	movl	%eax, -168(%ebp)
	movl	-164(%ebp), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	jne	L134
	movl	$72, -36(%ebp)
	leal	L_psr$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$8, %eax
	testl	%eax, %eax
	je	L138
	movl	$144, -36(%ebp)
	jmp	L138
L139:
	leal	-168(%ebp), %eax
	addl	$4, %eax
	movl	%eax, 8(%esp)
	movl	-40(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	$0, (%esp)
	call	_sleft
	movl	-168(%ebp), %eax
	decl	%eax
	movl	%eax, -168(%ebp)
	movl	-164(%ebp), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	jne	L134
L138:
	decl	-36(%ebp)
	cmpl	$-1, -36(%ebp)
	jne	L139
L134:
	movl	-168(%ebp), %eax
	andl	$8388607, %eax
	movl	%eax, -168(%ebp)
	leal	L_iselect$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$4, %ecx
	movl	-168(%ebp), %eax
	movl	%eax, %edx
	xorl	-32(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$5, %ecx
	movl	-164(%ebp), %eax
	movl	%eax, %edx
	xorl	-32(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$6, %ecx
	movl	-160(%ebp), %eax
	movl	%eax, %edx
	xorl	-32(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$7, %ecx
	movl	-156(%ebp), %eax
	movl	%eax, %edx
	xorl	-32(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	leal	L_psr$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$8, %eax
	testl	%eax, %eax
	je	L141
	movl	-152(%ebp), %eax
	xorl	-32(%ebp), %eax
	movl	%eax, -152(%ebp)
	movl	-148(%ebp), %eax
	xorl	-32(%ebp), %eax
	movl	%eax, -148(%ebp)
	movl	-144(%ebp), %eax
	xorl	-32(%ebp), %eax
	movl	%eax, -144(%ebp)
	leal	-168(%ebp), %eax
	addl	$16, %eax
	movl	-168(%ebp), %edx
	movl	%eax, 8(%esp)
	movl	%edx, 4(%esp)
	movl	-32(%ebp), %eax
	movl	%eax, (%esp)
	call	_store_minor_result
L141:
	addl	$204, %esp
	popl	%ebx
	popl	%esi
	popl	%edi
	leave
	ret
.globl _fpx
_fpx:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$72, %esp
	leal	-32(%ebp), %edx
	movl	8(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	_burst_read2
	movl	-32(%ebp), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	je	L144
	movl	$4193280, -48(%ebp)
	jmp	L146
L144:
	movl	$0, -48(%ebp)
L146:
	movl	-48(%ebp), %eax
	movl	%eax, -12(%ebp)
	movl	-32(%ebp), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	je	L147
	movl	$16777215, -44(%ebp)
	jmp	L149
L147:
	movl	$0, -44(%ebp)
L149:
	movl	-44(%ebp), %eax
	movl	%eax, -16(%ebp)
	movl	-32(%ebp), %eax
	xorl	-16(%ebp), %eax
	andl	$2048, %eax
	testl	%eax, %eax
	je	L150
	movl	-32(%ebp), %eax
	xorl	-16(%ebp), %eax
	andl	$4194304, %eax
	testl	%eax, %eax
	jne	L150
	xorl	$4193280, -12(%ebp)
L150:
	movl	-16(%ebp), %eax
	movl	%eax, -20(%ebp)
	movl	-28(%ebp), %eax
	movl	%eax, %edx
	sall	$12, %edx
	movl	-16(%ebp), %eax
	andl	$4095, %eax
	orl	%edx, %eax
	andl	$16777215, %eax
	movl	%eax, -24(%ebp)
	movl	-32(%ebp), %eax
	movl	%eax, %edx
	sall	$12, %edx
	movl	-28(%ebp), %eax
	shrl	$12, %eax
	orl	%edx, %eax
	andl	$16777215, %eax
	movl	%eax, -28(%ebp)
	movl	-32(%ebp), %eax
	movl	%eax, %edx
	andl	$12582912, %edx
	movl	-32(%ebp), %eax
	shrl	$12, %eax
	andl	$1023, %eax
	orl	%edx, %eax
	orl	-12(%ebp), %eax
	movl	%eax, -32(%ebp)
	movl	-32(%ebp), %eax
	movl	%eax, %edx
	movl	12(%ebp), %eax
	movl	%edx, (%eax)
	movl	12(%ebp), %edx
	addl	$4, %edx
	movl	-28(%ebp), %eax
	movl	%eax, (%edx)
	movl	12(%ebp), %edx
	addl	$8, %edx
	movl	-24(%ebp), %eax
	movl	%eax, (%edx)
	movl	12(%ebp), %edx
	addl	$12, %edx
	movl	-20(%ebp), %eax
	movl	%eax, (%edx)
	leave
	ret
.globl _fpp
_fpp:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$40, %esp
	movl	12(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -20(%ebp)
	movl	12(%ebp), %eax
	addl	$4, %eax
	movl	(%eax), %eax
	movl	%eax, -16(%ebp)
	movl	12(%ebp), %eax
	addl	$8, %eax
	movl	(%eax), %eax
	movl	%eax, -12(%ebp)
	movl	-20(%ebp), %eax
	movl	%eax, %edx
	andl	$12582912, %edx
	movl	-20(%ebp), %eax
	andl	$1023, %eax
	sall	$12, %eax
	orl	%eax, %edx
	movl	-16(%ebp), %eax
	shrl	$12, %eax
	orl	%edx, %eax
	andl	$16777215, %eax
	movl	%eax, -20(%ebp)
	movl	-16(%ebp), %eax
	movl	%eax, %edx
	sall	$12, %edx
	movl	-12(%ebp), %eax
	shrl	$12, %eax
	orl	%edx, %eax
	andl	$16777215, %eax
	movl	%eax, -16(%ebp)
	leal	-20(%ebp), %edx
	movl	8(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	_burst_write2
	leave
	ret
	.section __IMPORT,__pointers,non_lazy_symbol_pointers
L__register$non_lazy_ptr:
	.indirect_symbol __register
	.long	0
L_psr$non_lazy_ptr:
	.indirect_symbol _psr
	.long	0
L_iselect$non_lazy_ptr:
	.indirect_symbol _iselect
	.long	0
	.subsections_via_symbols
