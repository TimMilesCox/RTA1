	.text
_gshiftr:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$56, %esp
	movl	8(%ebp), %ecx
	movl	$715827883, -44(%ebp)
	movl	-44(%ebp), %eax
	imull	%ecx
	sarl	$2, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	subl	%eax, %edx
	movl	12(%ebp), %eax
	subl	%edx, %eax
	decl	%eax
	movl	%eax, -12(%ebp)
	movl	8(%ebp), %ecx
	movl	$715827883, -44(%ebp)
	movl	-44(%ebp), %eax
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
	movl	%eax, -16(%ebp)
	movl	$24, %eax
	subl	-16(%ebp), %eax
	movl	%eax, -20(%ebp)
	movl	16(%ebp), %eax
	movl	%eax, -24(%ebp)
	movl	$0, -28(%ebp)
	movl	12(%ebp), %eax
	movl	%eax, -32(%ebp)
	cmpl	$0, -12(%ebp)
	js	L4
	movl	-12(%ebp), %eax
	sall	$2, %eax
	addl	20(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -24(%ebp)
	decl	-12(%ebp)
	jmp	L4
L5:
	movl	-16(%ebp), %ecx
	movl	-24(%ebp), %eax
	sarl	%cl, %eax
	movl	%eax, -28(%ebp)
	movl	16(%ebp), %eax
	movl	%eax, -24(%ebp)
	cmpl	$0, -12(%ebp)
	js	L6
	movl	-12(%ebp), %eax
	sall	$2, %eax
	addl	20(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -24(%ebp)
	decl	-12(%ebp)
L6:
	movl	-20(%ebp), %ecx
	movl	-24(%ebp), %eax
	sall	%cl, %eax
	orl	%eax, -28(%ebp)
	movl	-32(%ebp), %eax
	sall	$2, %eax
	movl	%eax, %edx
	addl	20(%ebp), %edx
	movl	-28(%ebp), %eax
	andl	$16777215, %eax
	movl	%eax, (%edx)
L4:
	decl	-32(%ebp)
	cmpl	$-1, -32(%ebp)
	jne	L5
	leave
	ret
_gshiftl:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$56, %esp
	movl	8(%ebp), %ecx
	movl	$715827883, -44(%ebp)
	movl	-44(%ebp), %eax
	imull	%ecx
	sarl	$2, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	movl	%edx, %ecx
	subl	%eax, %ecx
	movl	%ecx, %eax
	movl	%eax, -12(%ebp)
	movl	8(%ebp), %ecx
	movl	$715827883, -44(%ebp)
	movl	-44(%ebp), %eax
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
	movl	%eax, -16(%ebp)
	movl	$24, %eax
	subl	-16(%ebp), %eax
	movl	%eax, -20(%ebp)
	movl	$0, -24(%ebp)
	movl	16(%ebp), %eax
	movl	%eax, -28(%ebp)
	movl	$0, -32(%ebp)
	movl	-12(%ebp), %eax
	cmpl	12(%ebp), %eax
	jge	L13
	movl	-12(%ebp), %eax
	sall	$2, %eax
	addl	20(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -28(%ebp)
	incl	-12(%ebp)
	jmp	L13
L14:
	movl	-16(%ebp), %ecx
	movl	-28(%ebp), %eax
	sall	%cl, %eax
	movl	%eax, -32(%ebp)
	movl	16(%ebp), %eax
	movl	%eax, -28(%ebp)
	movl	-12(%ebp), %eax
	cmpl	12(%ebp), %eax
	jge	L15
	movl	-12(%ebp), %eax
	sall	$2, %eax
	addl	20(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -28(%ebp)
	incl	-12(%ebp)
L15:
	movl	-20(%ebp), %ecx
	movl	-28(%ebp), %eax
	sarl	%cl, %eax
	orl	%eax, -32(%ebp)
	movl	-24(%ebp), %eax
	sall	$2, %eax
	movl	%eax, %edx
	addl	20(%ebp), %edx
	movl	-32(%ebp), %eax
	andl	$16777215, %eax
	movl	%eax, (%edx)
	incl	-24(%ebp)
L13:
	movl	-24(%ebp), %eax
	cmpl	12(%ebp), %eax
	jl	L14
	leave
	ret
.globl _sar
_sar:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	pushl	%ebx
	call	L21
"L00000000001$pb":
L21:
	popl	%ebx
	leal	L_iselect$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %esi
	orl	$4, %esi
	leal	L_iselect$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$4, %edx
	leal	L__register$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	8(%ebp), %ecx
	sarl	%cl, %eax
	movl	%eax, %edx
	andl	$16777215, %edx
	leal	L__register$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%esi,4)
	popl	%ebx
	popl	%esi
	leave
	ret
.globl _sbr
_sbr:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	pushl	%ebx
	call	L24
"L00000000002$pb":
L24:
	popl	%ebx
	leal	L_iselect$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %esi
	orl	$5, %esi
	leal	L_iselect$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$5, %edx
	leal	L__register$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	8(%ebp), %ecx
	sarl	%cl, %eax
	movl	%eax, %edx
	andl	$16777215, %edx
	leal	L__register$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%esi,4)
	popl	%ebx
	popl	%esi
	leave
	ret
.globl _dsr
_dsr:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$20, %esp
	call	L27
"L00000000003$pb":
L27:
	popl	%ebx
	leal	L_iselect$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	orl	$4, %eax
	sall	$2, %eax
	movl	%eax, %edx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	leal	(%edx,%eax), %eax
	movl	%eax, 12(%esp)
	movl	$0, 8(%esp)
	movl	$2, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_gshiftr
	addl	$20, %esp
	popl	%ebx
	leave
	ret
.globl _sal
_sal:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	pushl	%ebx
	call	L30
"L00000000004$pb":
L30:
	popl	%ebx
	leal	L_iselect$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %esi
	orl	$4, %esi
	leal	L_iselect$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$4, %edx
	leal	L__register$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	8(%ebp), %ecx
	sall	%cl, %eax
	movl	%eax, %edx
	andl	$16777215, %edx
	leal	L__register$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%esi,4)
	popl	%ebx
	popl	%esi
	leave
	ret
.globl _sbl
_sbl:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	pushl	%ebx
	call	L33
"L00000000005$pb":
L33:
	popl	%ebx
	leal	L_iselect$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %esi
	orl	$5, %esi
	leal	L_iselect$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$5, %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	8(%ebp), %ecx
	sall	%cl, %eax
	movl	%eax, %edx
	andl	$16777215, %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%esi,4)
	popl	%ebx
	popl	%esi
	leave
	ret
.globl _dsl
_dsl:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$20, %esp
	call	L36
"L00000000006$pb":
L36:
	popl	%ebx
	leal	L_iselect$non_lazy_ptr-"L00000000006$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	orl	$4, %eax
	sall	$2, %eax
	movl	%eax, %edx
	leal	L__register$non_lazy_ptr-"L00000000006$pb"(%ebx), %eax
	movl	(%eax), %eax
	leal	(%edx,%eax), %eax
	movl	%eax, 12(%esp)
	movl	$0, 8(%esp)
	movl	$2, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_gshiftl
	addl	$20, %esp
	popl	%ebx
	leave
	ret
.globl _rar
_rar:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$20, %esp
	call	L39
"L00000000007$pb":
L39:
	popl	%ebx
	leal	L_iselect$non_lazy_ptr-"L00000000007$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	orl	$4, %eax
	sall	$2, %eax
	movl	%eax, %edx
	leal	L__register$non_lazy_ptr-"L00000000007$pb"(%ebx), %eax
	movl	(%eax), %eax
	leal	(%edx,%eax), %ecx
	leal	L_iselect$non_lazy_ptr-"L00000000007$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$4, %edx
	leal	L__register$non_lazy_ptr-"L00000000007$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%ecx, 12(%esp)
	movl	%eax, 8(%esp)
	movl	$1, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_gshiftr
	addl	$20, %esp
	popl	%ebx
	leave
	ret
.globl _rbr
_rbr:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$20, %esp
	call	L42
"L00000000008$pb":
L42:
	popl	%ebx
	leal	L_iselect$non_lazy_ptr-"L00000000008$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	orl	$5, %eax
	sall	$2, %eax
	movl	%eax, %edx
	leal	L__register$non_lazy_ptr-"L00000000008$pb"(%ebx), %eax
	movl	(%eax), %eax
	leal	(%edx,%eax), %ecx
	leal	L_iselect$non_lazy_ptr-"L00000000008$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$5, %edx
	leal	L__register$non_lazy_ptr-"L00000000008$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%ecx, 12(%esp)
	movl	%eax, 8(%esp)
	movl	$1, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_gshiftr
	addl	$20, %esp
	popl	%ebx
	leave
	ret
.globl _drr
_drr:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$76, %esp
	call	L45
"L00000000009$pb":
L45:
	popl	%ebx
	leal	L_iselect$non_lazy_ptr-"L00000000009$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$4, %edx
	leal	L__register$non_lazy_ptr-"L00000000009$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %esi
	leal	L_iselect$non_lazy_ptr-"L00000000009$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$5, %edx
	leal	L__register$non_lazy_ptr-"L00000000009$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %edi
	leal	L_iselect$non_lazy_ptr-"L00000000009$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$4, %edx
	leal	L__register$non_lazy_ptr-"L00000000009$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %ecx
	leal	L_iselect$non_lazy_ptr-"L00000000009$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$5, %edx
	leal	L__register$non_lazy_ptr-"L00000000009$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%esi, -40(%ebp)
	movl	%edi, -36(%ebp)
	movl	%ecx, -32(%ebp)
	movl	%eax, -28(%ebp)
	movl	8(%ebp), %ecx
	movl	$715827883, -60(%ebp)
	movl	-60(%ebp), %eax
	imull	%ecx
	sarl	$3, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	movl	%edx, %esi
	subl	%eax, %esi
	movl	%esi, -44(%ebp)
	movl	-44(%ebp), %eax
	addl	%eax, %eax
	addl	-44(%ebp), %eax
	sall	$4, %eax
	movl	%ecx, %edx
	subl	%eax, %edx
	movl	%edx, -44(%ebp)
	leal	-40(%ebp), %eax
	movl	%eax, 12(%esp)
	movl	$0, 8(%esp)
	movl	$4, 4(%esp)
	movl	-44(%ebp), %esi
	movl	%esi, (%esp)
	call	_gshiftr
	leal	L_iselect$non_lazy_ptr-"L00000000009$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$4, %ecx
	movl	-32(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000009$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000009$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$5, %ecx
	movl	-28(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000009$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	addl	$76, %esp
	popl	%ebx
	popl	%esi
	popl	%edi
	leave
	ret
.globl _ral
_ral:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$20, %esp
	call	L48
"L00000000010$pb":
L48:
	popl	%ebx
	leal	L_iselect$non_lazy_ptr-"L00000000010$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	orl	$4, %eax
	sall	$2, %eax
	movl	%eax, %edx
	leal	L__register$non_lazy_ptr-"L00000000010$pb"(%ebx), %eax
	movl	(%eax), %eax
	leal	(%edx,%eax), %ecx
	leal	L_iselect$non_lazy_ptr-"L00000000010$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$4, %edx
	leal	L__register$non_lazy_ptr-"L00000000010$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%ecx, 12(%esp)
	movl	%eax, 8(%esp)
	movl	$1, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_gshiftl
	addl	$20, %esp
	popl	%ebx
	leave
	ret
.globl _rbl
_rbl:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$20, %esp
	call	L51
"L00000000011$pb":
L51:
	popl	%ebx
	leal	L_iselect$non_lazy_ptr-"L00000000011$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	orl	$5, %eax
	sall	$2, %eax
	movl	%eax, %edx
	leal	L__register$non_lazy_ptr-"L00000000011$pb"(%ebx), %eax
	movl	(%eax), %eax
	leal	(%edx,%eax), %ecx
	leal	L_iselect$non_lazy_ptr-"L00000000011$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$5, %edx
	leal	L__register$non_lazy_ptr-"L00000000011$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%ecx, 12(%esp)
	movl	%eax, 8(%esp)
	movl	$1, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_gshiftl
	addl	$20, %esp
	popl	%ebx
	leave
	ret
.globl _drl
_drl:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$76, %esp
	call	L54
"L00000000012$pb":
L54:
	popl	%ebx
	leal	L_iselect$non_lazy_ptr-"L00000000012$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$4, %edx
	leal	L__register$non_lazy_ptr-"L00000000012$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %esi
	leal	L_iselect$non_lazy_ptr-"L00000000012$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$5, %edx
	leal	L__register$non_lazy_ptr-"L00000000012$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %edi
	leal	L_iselect$non_lazy_ptr-"L00000000012$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$4, %edx
	leal	L__register$non_lazy_ptr-"L00000000012$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %ecx
	leal	L_iselect$non_lazy_ptr-"L00000000012$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$5, %edx
	leal	L__register$non_lazy_ptr-"L00000000012$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%esi, -40(%ebp)
	movl	%edi, -36(%ebp)
	movl	%ecx, -32(%ebp)
	movl	%eax, -28(%ebp)
	movl	8(%ebp), %ecx
	movl	$715827883, -60(%ebp)
	movl	-60(%ebp), %eax
	imull	%ecx
	sarl	$3, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	movl	%edx, %esi
	subl	%eax, %esi
	movl	%esi, -44(%ebp)
	movl	-44(%ebp), %eax
	addl	%eax, %eax
	addl	-44(%ebp), %eax
	sall	$4, %eax
	movl	%ecx, %edx
	subl	%eax, %edx
	movl	%edx, -44(%ebp)
	leal	-40(%ebp), %eax
	movl	%eax, 12(%esp)
	movl	$0, 8(%esp)
	movl	$4, 4(%esp)
	movl	-44(%ebp), %esi
	movl	%esi, (%esp)
	call	_gshiftl
	leal	L_iselect$non_lazy_ptr-"L00000000012$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$4, %ecx
	movl	-40(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000012$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000012$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$5, %ecx
	movl	-36(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000012$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	addl	$76, %esp
	popl	%ebx
	popl	%esi
	popl	%edi
	leave
	ret
.globl _saa
_saa:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$36, %esp
	call	L57
"L00000000013$pb":
L57:
	popl	%ebx
	leal	L_iselect$non_lazy_ptr-"L00000000013$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$4, %edx
	leal	L__register$non_lazy_ptr-"L00000000013$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	sarl	$23, %eax
	andl	$1, %eax
	negl	%eax
	andl	$16777215, %eax
	movl	%eax, -12(%ebp)
	leal	L_iselect$non_lazy_ptr-"L00000000013$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	orl	$4, %eax
	sall	$2, %eax
	movl	%eax, %edx
	leal	L__register$non_lazy_ptr-"L00000000013$pb"(%ebx), %eax
	movl	(%eax), %eax
	leal	(%edx,%eax), %eax
	movl	%eax, 12(%esp)
	movl	-12(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	$1, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_gshiftr
	addl	$36, %esp
	popl	%ebx
	leave
	ret
.globl _sba
_sba:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$36, %esp
	call	L60
"L00000000014$pb":
L60:
	popl	%ebx
	leal	L_iselect$non_lazy_ptr-"L00000000014$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$5, %edx
	leal	L__register$non_lazy_ptr-"L00000000014$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	sarl	$23, %eax
	andl	$1, %eax
	negl	%eax
	andl	$16777215, %eax
	movl	%eax, -12(%ebp)
	leal	L_iselect$non_lazy_ptr-"L00000000014$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	orl	$5, %eax
	sall	$2, %eax
	movl	%eax, %edx
	leal	L__register$non_lazy_ptr-"L00000000014$pb"(%ebx), %eax
	movl	(%eax), %eax
	leal	(%edx,%eax), %eax
	movl	%eax, 12(%esp)
	movl	-12(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	$1, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_gshiftr
	addl	$36, %esp
	popl	%ebx
	leave
	ret
.globl _dsa
_dsa:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$36, %esp
	call	L63
"L00000000015$pb":
L63:
	popl	%ebx
	leal	L_iselect$non_lazy_ptr-"L00000000015$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$4, %edx
	leal	L__register$non_lazy_ptr-"L00000000015$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	sarl	$23, %eax
	andl	$1, %eax
	negl	%eax
	andl	$16777215, %eax
	movl	%eax, -12(%ebp)
	leal	L_iselect$non_lazy_ptr-"L00000000015$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	orl	$4, %eax
	sall	$2, %eax
	movl	%eax, %edx
	leal	L__register$non_lazy_ptr-"L00000000015$pb"(%ebx), %eax
	movl	(%eax), %eax
	leal	(%edx,%eax), %eax
	movl	%eax, 12(%esp)
	movl	-12(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	$2, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_gshiftr
	addl	$36, %esp
	popl	%ebx
	leave
	ret
.globl _lsc
_lsc:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$36, %esp
	call	L70
"L00000000016$pb":
L70:
	popl	%ebx
	movl	$7, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_operand_read
	movl	%eax, -12(%ebp)
	movl	$0, -16(%ebp)
	movl	-12(%ebp), %eax
	andl	$8388608, %eax
	movl	%eax, -20(%ebp)
	movl	-12(%ebp), %eax
	sarl	$23, %eax
	movl	%eax, -24(%ebp)
	jmp	L65
L66:
	movl	-20(%ebp), %eax
	xorl	-12(%ebp), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	jne	L67
	incl	-16(%ebp)
	sall	-12(%ebp)
	movl	-24(%ebp), %eax
	orl	%eax, -12(%ebp)
L65:
	cmpl	$23, -16(%ebp)
	jle	L66
L67:
	leal	L_iselect$non_lazy_ptr-"L00000000016$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$4, %ecx
	movl	-12(%ebp), %edx
	andl	$16777215, %edx
	leal	L__register$non_lazy_ptr-"L00000000016$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000016$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$5, %ecx
	leal	L__register$non_lazy_ptr-"L00000000016$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	-16(%ebp), %edx
	movl	%edx, (%eax,%ecx,4)
	addl	$36, %esp
	popl	%ebx
	leave
	ret
.globl _da
_da:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$36, %esp
	call	L73
"L00000000017$pb":
L73:
	popl	%ebx
	movl	8(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	-20(%ebp), %eax
	movl	%eax, (%esp)
	call	_burst_read2
	movl	-16(%ebp), %ecx
	leal	L_iselect$non_lazy_ptr-"L00000000017$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$5, %edx
	leal	L__register$non_lazy_ptr-"L00000000017$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	leal	(%ecx,%eax), %eax
	movl	%eax, -12(%ebp)
	leal	L_iselect$non_lazy_ptr-"L00000000017$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$5, %ecx
	movl	-12(%ebp), %edx
	andl	$16777215, %edx
	leal	L__register$non_lazy_ptr-"L00000000017$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	sarl	$24, -12(%ebp)
	leal	L_iselect$non_lazy_ptr-"L00000000017$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$4, %edx
	leal	L__register$non_lazy_ptr-"L00000000017$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	addl	%eax, -12(%ebp)
	movl	-20(%ebp), %eax
	addl	%eax, -12(%ebp)
	leal	L_iselect$non_lazy_ptr-"L00000000017$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$4, %ecx
	movl	-12(%ebp), %edx
	andl	$16777215, %edx
	leal	L__register$non_lazy_ptr-"L00000000017$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	sarl	$24, -12(%ebp)
	leal	L_psr$non_lazy_ptr-"L00000000017$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	andl	$16777214, %edx
	leal	L_psr$non_lazy_ptr-"L00000000017$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax)
	leal	L_psr$non_lazy_ptr-"L00000000017$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	-12(%ebp), %edx
	leal	L_psr$non_lazy_ptr-"L00000000017$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax)
	addl	$36, %esp
	popl	%ebx
	leave
	ret
.globl _dan
_dan:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$36, %esp
	call	L76
"L00000000018$pb":
L76:
	popl	%ebx
	movl	$1, -12(%ebp)
	movl	8(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	-20(%ebp), %eax
	movl	%eax, (%esp)
	call	_burst_read2
	movl	-20(%ebp), %eax
	xorl	$16777215, %eax
	movl	%eax, -20(%ebp)
	movl	-16(%ebp), %eax
	xorl	$16777215, %eax
	movl	%eax, -16(%ebp)
	leal	L_iselect$non_lazy_ptr-"L00000000018$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$5, %edx
	leal	L__register$non_lazy_ptr-"L00000000018$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	addl	%eax, -12(%ebp)
	movl	-16(%ebp), %eax
	addl	%eax, -12(%ebp)
	leal	L_iselect$non_lazy_ptr-"L00000000018$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$5, %ecx
	movl	-12(%ebp), %edx
	andl	$16777215, %edx
	leal	L__register$non_lazy_ptr-"L00000000018$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	sarl	$24, -12(%ebp)
	leal	L_iselect$non_lazy_ptr-"L00000000018$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$4, %edx
	leal	L__register$non_lazy_ptr-"L00000000018$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	addl	%eax, -12(%ebp)
	movl	-20(%ebp), %eax
	addl	%eax, -12(%ebp)
	leal	L_iselect$non_lazy_ptr-"L00000000018$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$4, %ecx
	movl	-12(%ebp), %edx
	andl	$16777215, %edx
	leal	L__register$non_lazy_ptr-"L00000000018$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	sarl	$24, -12(%ebp)
	leal	L_psr$non_lazy_ptr-"L00000000018$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	andl	$16777214, %edx
	leal	L_psr$non_lazy_ptr-"L00000000018$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax)
	leal	L_psr$non_lazy_ptr-"L00000000018$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	-12(%ebp), %edx
	leal	L_psr$non_lazy_ptr-"L00000000018$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax)
	addl	$36, %esp
	popl	%ebx
	leave
	ret
.globl _dlsc
_dlsc:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$36, %esp
	call	L83
"L00000000019$pb":
L83:
	popl	%ebx
	movl	$0, -12(%ebp)
	movl	8(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	-24(%ebp), %eax
	movl	%eax, (%esp)
	call	_burst_read2
	movl	-24(%ebp), %eax
	andl	$8388608, %eax
	movl	%eax, -16(%ebp)
	jmp	L78
L79:
	movl	-24(%ebp), %eax
	xorl	-16(%ebp), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	jne	L80
	incl	-12(%ebp)
	leal	-24(%ebp), %eax
	movl	%eax, 12(%esp)
	movl	-16(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	$2, 4(%esp)
	movl	$1, (%esp)
	call	_gshiftl
L78:
	cmpl	$47, -12(%ebp)
	jle	L79
L80:
	leal	L_iselect$non_lazy_ptr-"L00000000019$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$4, %ecx
	movl	-24(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000019$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000019$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$5, %ecx
	movl	-20(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000019$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000019$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$6, %ecx
	leal	L__register$non_lazy_ptr-"L00000000019$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	-12(%ebp), %edx
	movl	%edx, (%eax,%ecx,4)
	addl	$36, %esp
	popl	%ebx
	leave
	ret
.globl _or
_or:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$4, %esp
	call	L86
"L00000000020$pb":
L86:
	popl	%ebx
	leal	L_iselect$non_lazy_ptr-"L00000000020$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$4, %ecx
	leal	L_iselect$non_lazy_ptr-"L00000000020$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$4, %edx
	leal	L__register$non_lazy_ptr-"L00000000020$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, %edx
	orl	8(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000020$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	addl	$4, %esp
	popl	%ebx
	leave
	ret
.globl _orb
_orb:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$4, %esp
	call	L89
"L00000000021$pb":
L89:
	popl	%ebx
	leal	L_iselect$non_lazy_ptr-"L00000000021$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$5, %ecx
	leal	L_iselect$non_lazy_ptr-"L00000000021$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$5, %edx
	leal	L__register$non_lazy_ptr-"L00000000021$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, %edx
	orl	8(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000021$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	addl	$4, %esp
	popl	%ebx
	leave
	ret
.globl _and
_and:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$4, %esp
	call	L92
"L00000000022$pb":
L92:
	popl	%ebx
	leal	L_iselect$non_lazy_ptr-"L00000000022$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$4, %ecx
	leal	L_iselect$non_lazy_ptr-"L00000000022$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$4, %edx
	leal	L__register$non_lazy_ptr-"L00000000022$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, %edx
	andl	8(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000022$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	addl	$4, %esp
	popl	%ebx
	leave
	ret
.globl _andb
_andb:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$4, %esp
	call	L95
"L00000000023$pb":
L95:
	popl	%ebx
	leal	L_iselect$non_lazy_ptr-"L00000000023$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$5, %ecx
	leal	L_iselect$non_lazy_ptr-"L00000000023$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$5, %edx
	leal	L__register$non_lazy_ptr-"L00000000023$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, %edx
	andl	8(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000023$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	addl	$4, %esp
	popl	%ebx
	leave
	ret
.globl _xor
_xor:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$4, %esp
	call	L98
"L00000000024$pb":
L98:
	popl	%ebx
	leal	L_iselect$non_lazy_ptr-"L00000000024$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$4, %ecx
	leal	L_iselect$non_lazy_ptr-"L00000000024$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$4, %edx
	leal	L__register$non_lazy_ptr-"L00000000024$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, %edx
	xorl	8(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000024$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	addl	$4, %esp
	popl	%ebx
	leave
	ret
.globl _xorb
_xorb:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$4, %esp
	call	L101
"L00000000025$pb":
L101:
	popl	%ebx
	leal	L_iselect$non_lazy_ptr-"L00000000025$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$5, %ecx
	leal	L_iselect$non_lazy_ptr-"L00000000025$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$5, %edx
	leal	L__register$non_lazy_ptr-"L00000000025$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, %edx
	xorl	8(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000025$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	addl	$4, %esp
	popl	%ebx
	leave
	ret
.globl _aa
_aa:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$20, %esp
	call	L104
"L00000000026$pb":
L104:
	popl	%ebx
	leal	L_iselect$non_lazy_ptr-"L00000000026$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$4, %edx
	leal	L__register$non_lazy_ptr-"L00000000026$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	addl	8(%ebp), %eax
	movl	%eax, -12(%ebp)
	leal	L_iselect$non_lazy_ptr-"L00000000026$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$4, %ecx
	movl	-12(%ebp), %edx
	andl	$16777215, %edx
	leal	L__register$non_lazy_ptr-"L00000000026$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	sarl	$24, -12(%ebp)
	leal	L_psr$non_lazy_ptr-"L00000000026$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	andl	$16777214, %edx
	leal	L_psr$non_lazy_ptr-"L00000000026$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax)
	leal	L_psr$non_lazy_ptr-"L00000000026$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	-12(%ebp), %edx
	leal	L_psr$non_lazy_ptr-"L00000000026$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax)
	addl	$20, %esp
	popl	%ebx
	leave
	ret
.globl _ab
_ab:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$20, %esp
	call	L107
"L00000000027$pb":
L107:
	popl	%ebx
	leal	L_iselect$non_lazy_ptr-"L00000000027$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$5, %edx
	leal	L__register$non_lazy_ptr-"L00000000027$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	addl	8(%ebp), %eax
	movl	%eax, -12(%ebp)
	leal	L_iselect$non_lazy_ptr-"L00000000027$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$5, %ecx
	movl	-12(%ebp), %edx
	andl	$16777215, %edx
	leal	L__register$non_lazy_ptr-"L00000000027$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	sarl	$24, -12(%ebp)
	leal	L_psr$non_lazy_ptr-"L00000000027$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	andl	$16777214, %edx
	leal	L_psr$non_lazy_ptr-"L00000000027$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax)
	leal	L_psr$non_lazy_ptr-"L00000000027$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	-12(%ebp), %edx
	leal	L_psr$non_lazy_ptr-"L00000000027$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax)
	addl	$20, %esp
	popl	%ebx
	leave
	ret
.globl _ana
_ana:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$20, %esp
	call	L110
"L00000000028$pb":
L110:
	popl	%ebx
	movl	8(%ebp), %eax
	xorl	$16777215, %eax
	movl	%eax, -12(%ebp)
	incl	-12(%ebp)
	leal	L_iselect$non_lazy_ptr-"L00000000028$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$4, %edx
	leal	L__register$non_lazy_ptr-"L00000000028$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	addl	%eax, -12(%ebp)
	leal	L_iselect$non_lazy_ptr-"L00000000028$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$4, %ecx
	movl	-12(%ebp), %edx
	andl	$16777215, %edx
	leal	L__register$non_lazy_ptr-"L00000000028$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	sarl	$24, -12(%ebp)
	leal	L_psr$non_lazy_ptr-"L00000000028$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	andl	$16777214, %edx
	leal	L_psr$non_lazy_ptr-"L00000000028$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax)
	leal	L_psr$non_lazy_ptr-"L00000000028$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	-12(%ebp), %edx
	leal	L_psr$non_lazy_ptr-"L00000000028$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax)
	addl	$20, %esp
	popl	%ebx
	leave
	ret
.globl _anb
_anb:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$20, %esp
	call	L113
"L00000000029$pb":
L113:
	popl	%ebx
	movl	8(%ebp), %eax
	xorl	$16777215, %eax
	movl	%eax, -12(%ebp)
	incl	-12(%ebp)
	leal	L_iselect$non_lazy_ptr-"L00000000029$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$5, %edx
	leal	L__register$non_lazy_ptr-"L00000000029$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	addl	%eax, -12(%ebp)
	leal	L_iselect$non_lazy_ptr-"L00000000029$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$5, %ecx
	movl	-12(%ebp), %edx
	andl	$16777215, %edx
	leal	L__register$non_lazy_ptr-"L00000000029$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	sarl	$24, -12(%ebp)
	leal	L_psr$non_lazy_ptr-"L00000000029$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	andl	$16777214, %edx
	leal	L_psr$non_lazy_ptr-"L00000000029$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax)
	leal	L_psr$non_lazy_ptr-"L00000000029$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	-12(%ebp), %edx
	leal	L_psr$non_lazy_ptr-"L00000000029$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax)
	addl	$20, %esp
	popl	%ebx
	leave
	ret
.globl _m
_m:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$36, %esp
	call	L126
"L00000000030$pb":
L126:
	popl	%ebx
	movl	$0, -28(%ebp)
	movl	$0, -24(%ebp)
	leal	L_iselect$non_lazy_ptr-"L00000000030$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$5, %edx
	leal	L__register$non_lazy_ptr-"L00000000030$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, -36(%ebp)
	movl	8(%ebp), %eax
	movl	%eax, -32(%ebp)
	movl	$0, -12(%ebp)
	movl	$24, -16(%ebp)
	movl	$0, -20(%ebp)
	movl	-36(%ebp), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	je	L115
	movl	$16777215, -12(%ebp)
	movl	-36(%ebp), %eax
	xorl	-12(%ebp), %eax
	movl	%eax, -36(%ebp)
	movl	-36(%ebp), %eax
	incl	%eax
	movl	%eax, -36(%ebp)
L115:
	movl	-32(%ebp), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	je	L119
	xorl	$16777215, -12(%ebp)
	movl	-32(%ebp), %eax
	xorl	$16777215, %eax
	movl	%eax, -32(%ebp)
	movl	-32(%ebp), %eax
	incl	%eax
	movl	%eax, -32(%ebp)
	jmp	L119
L120:
	movl	-28(%ebp), %eax
	addl	%eax, %eax
	movl	%eax, -28(%ebp)
	movl	-28(%ebp), %edx
	movl	-24(%ebp), %eax
	sarl	$23, %eax
	orl	%edx, %eax
	movl	%eax, -28(%ebp)
	movl	-24(%ebp), %eax
	addl	%eax, %eax
	movl	%eax, -24(%ebp)
	movl	-24(%ebp), %eax
	andl	$16777215, %eax
	movl	%eax, -24(%ebp)
	movl	-32(%ebp), %eax
	addl	%eax, %eax
	movl	%eax, -32(%ebp)
	movl	-32(%ebp), %eax
	andl	$16777216, %eax
	testl	%eax, %eax
	je	L119
	movl	-36(%ebp), %edx
	movl	-24(%ebp), %eax
	leal	(%edx,%eax), %eax
	movl	%eax, -20(%ebp)
	movl	-20(%ebp), %eax
	andl	$16777215, %eax
	movl	%eax, -24(%ebp)
	sarl	$24, -20(%ebp)
	movl	-28(%ebp), %eax
	addl	-20(%ebp), %eax
	movl	%eax, -28(%ebp)
L119:
	decl	-16(%ebp)
	cmpl	$-1, -16(%ebp)
	jne	L120
	cmpl	$0, -12(%ebp)
	je	L123
	movl	$1, -20(%ebp)
	movl	-24(%ebp), %eax
	xorl	$16777215, %eax
	addl	%eax, -20(%ebp)
	movl	-20(%ebp), %eax
	movl	%eax, -24(%ebp)
	sarl	$24, -20(%ebp)
	andl	$1, -20(%ebp)
	movl	-28(%ebp), %eax
	xorl	$16777215, %eax
	addl	%eax, -20(%ebp)
	movl	-20(%ebp), %eax
	movl	%eax, -28(%ebp)
L123:
	leal	L_iselect$non_lazy_ptr-"L00000000030$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$4, %ecx
	movl	-28(%ebp), %eax
	movl	%eax, %edx
	andl	$16777215, %edx
	leal	L__register$non_lazy_ptr-"L00000000030$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000030$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$5, %ecx
	movl	-24(%ebp), %eax
	movl	%eax, %edx
	andl	$16777215, %edx
	leal	L__register$non_lazy_ptr-"L00000000030$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	addl	$36, %esp
	popl	%ebx
	leave
	ret
.globl _mf
_mf:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$36, %esp
	call	L137
"L00000000031$pb":
L137:
	popl	%ebx
	movl	$0, -28(%ebp)
	movl	$0, -24(%ebp)
	leal	L_iselect$non_lazy_ptr-"L00000000031$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$5, %edx
	leal	L__register$non_lazy_ptr-"L00000000031$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, -36(%ebp)
	movl	8(%ebp), %eax
	movl	%eax, -32(%ebp)
	movl	$0, -12(%ebp)
	movl	$24, -16(%ebp)
	movl	$0, -20(%ebp)
	movl	-32(%ebp), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	je	L130
	xorl	$16777215, -12(%ebp)
	movl	-32(%ebp), %eax
	xorl	$16777215, %eax
	movl	%eax, -32(%ebp)
	movl	-32(%ebp), %eax
	incl	%eax
	movl	%eax, -32(%ebp)
	jmp	L130
L131:
	movl	-28(%ebp), %eax
	addl	%eax, %eax
	movl	%eax, -28(%ebp)
	movl	-28(%ebp), %edx
	movl	-24(%ebp), %eax
	sarl	$23, %eax
	orl	%edx, %eax
	movl	%eax, -28(%ebp)
	movl	-24(%ebp), %eax
	addl	%eax, %eax
	movl	%eax, -24(%ebp)
	movl	-24(%ebp), %eax
	andl	$16777215, %eax
	movl	%eax, -24(%ebp)
	movl	-32(%ebp), %eax
	addl	%eax, %eax
	movl	%eax, -32(%ebp)
	movl	-32(%ebp), %eax
	andl	$16777216, %eax
	testl	%eax, %eax
	je	L130
	movl	-36(%ebp), %edx
	movl	-24(%ebp), %eax
	leal	(%edx,%eax), %eax
	movl	%eax, -20(%ebp)
	movl	-20(%ebp), %eax
	andl	$16777215, %eax
	movl	%eax, -24(%ebp)
	sarl	$24, -20(%ebp)
	movl	-28(%ebp), %eax
	addl	-20(%ebp), %eax
	movl	%eax, -28(%ebp)
L130:
	decl	-16(%ebp)
	cmpl	$-1, -16(%ebp)
	jne	L131
	cmpl	$0, -12(%ebp)
	je	L134
	movl	$1, -20(%ebp)
	movl	-24(%ebp), %eax
	xorl	$16777215, %eax
	addl	%eax, -20(%ebp)
	movl	-20(%ebp), %eax
	movl	%eax, -24(%ebp)
	sarl	$24, -20(%ebp)
	andl	$1, -20(%ebp)
	movl	-28(%ebp), %eax
	xorl	$16777215, %eax
	addl	%eax, -20(%ebp)
	movl	-20(%ebp), %eax
	movl	%eax, -28(%ebp)
L134:
	leal	L_iselect$non_lazy_ptr-"L00000000031$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$4, %ecx
	movl	-28(%ebp), %eax
	movl	%eax, %edx
	andl	$16777215, %edx
	leal	L__register$non_lazy_ptr-"L00000000031$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000031$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$5, %ecx
	movl	-24(%ebp), %eax
	movl	%eax, %edx
	andl	$16777215, %edx
	leal	L__register$non_lazy_ptr-"L00000000031$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	addl	$36, %esp
	popl	%ebx
	leave
	ret
.globl _d
_d:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$52, %esp
	call	L157
"L00000000032$pb":
L157:
	popl	%ebx
	movl	$0, -12(%ebp)
	leal	L_iselect$non_lazy_ptr-"L00000000032$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$4, %edx
	leal	L__register$non_lazy_ptr-"L00000000032$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %ecx
	leal	L_iselect$non_lazy_ptr-"L00000000032$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$5, %edx
	leal	L__register$non_lazy_ptr-"L00000000032$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%ecx, -44(%ebp)
	movl	%eax, -40(%ebp)
	movl	8(%ebp), %eax
	movl	%eax, -52(%ebp)
	movl	$16777215, -48(%ebp)
	movl	-44(%ebp), %eax
	sarl	$23, %eax
	andl	$1, %eax
	negl	%eax
	andl	$16777215, %eax
	movl	%eax, -16(%ebp)
	movl	-52(%ebp), %eax
	sarl	$23, %eax
	andl	$1, %eax
	negl	%eax
	andl	$16777215, %eax
	movl	%eax, -20(%ebp)
	movl	$24, -24(%ebp)
	movl	$1, -32(%ebp)
	movl	$0, -36(%ebp)
	cmpl	$0, -16(%ebp)
	je	L139
	movl	-40(%ebp), %eax
	xorl	$16777215, %eax
	addl	%eax, -32(%ebp)
	movl	-32(%ebp), %eax
	andl	$16777215, %eax
	movl	%eax, -40(%ebp)
	sarl	$24, -32(%ebp)
	movl	-44(%ebp), %eax
	xorl	$16777215, %eax
	addl	%eax, -32(%ebp)
	movl	-32(%ebp), %eax
	andl	$16777215, %eax
	movl	%eax, -44(%ebp)
L139:
	cmpl	$0, -20(%ebp)
	je	L141
	movl	-52(%ebp), %eax
	decl	%eax
	movl	%eax, -52(%ebp)
	jmp	L144
L141:
	movl	-52(%ebp), %eax
	xorl	$16777215, %eax
	movl	%eax, -52(%ebp)
	jmp	L144
L145:
	movl	-52(%ebp), %eax
	cmpl	$16777215, %eax
	je	L148
	incl	-24(%ebp)
	movl	-52(%ebp), %eax
	addl	%eax, %eax
	movl	%eax, -52(%ebp)
	movl	-52(%ebp), %eax
	orl	$1, %eax
	movl	%eax, -52(%ebp)
	movl	-52(%ebp), %eax
	andl	$16777215, %eax
	movl	%eax, -52(%ebp)
L144:
	movl	-52(%ebp), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	jne	L145
	jmp	L148
L149:
	sall	-36(%ebp)
	movl	-12(%ebp), %eax
	sarl	$23, %eax
	andl	$1, %eax
	orl	%eax, -36(%ebp)
	sall	-12(%ebp)
	andl	$16777215, -12(%ebp)
	movl	-48(%ebp), %eax
	sarl	%eax
	movl	%eax, -48(%ebp)
	movl	-48(%ebp), %edx
	movl	-52(%ebp), %eax
	andl	$1, %eax
	sall	$23, %eax
	orl	%edx, %eax
	movl	%eax, -48(%ebp)
	movl	-52(%ebp), %eax
	sarl	%eax
	movl	%eax, -52(%ebp)
	movl	-52(%ebp), %eax
	orl	$8388608, %eax
	movl	%eax, -52(%ebp)
	movl	-40(%ebp), %edx
	movl	-48(%ebp), %eax
	leal	(%edx,%eax), %eax
	incl	%eax
	movl	%eax, -28(%ebp)
	movl	-28(%ebp), %eax
	sarl	$24, %eax
	movl	%eax, -32(%ebp)
	movl	-44(%ebp), %eax
	addl	%eax, -32(%ebp)
	movl	-52(%ebp), %eax
	addl	%eax, -32(%ebp)
	movl	-32(%ebp), %eax
	andl	$16777216, %eax
	testl	%eax, %eax
	je	L148
	movl	-28(%ebp), %eax
	andl	$16777215, %eax
	movl	%eax, -40(%ebp)
	movl	-32(%ebp), %eax
	andl	$16777215, %eax
	movl	%eax, -44(%ebp)
	orl	$1, -12(%ebp)
L148:
	decl	-24(%ebp)
	cmpl	$-1, -24(%ebp)
	jne	L149
	cmpl	$0, -16(%ebp)
	je	L152
	movl	-40(%ebp), %eax
	xorl	-16(%ebp), %eax
	movl	%eax, -40(%ebp)
	movl	-40(%ebp), %eax
	incl	%eax
	movl	%eax, -40(%ebp)
L152:
	movl	-20(%ebp), %eax
	xorl	%eax, -16(%ebp)
	cmpl	$0, -16(%ebp)
	je	L154
	movl	-16(%ebp), %eax
	xorl	%eax, -12(%ebp)
	incl	-12(%ebp)
	movl	-16(%ebp), %eax
	xorl	%eax, -36(%ebp)
	movl	-12(%ebp), %eax
	sarl	$24, %eax
	andl	$1, %eax
	addl	%eax, -36(%ebp)
L154:
	leal	L_iselect$non_lazy_ptr-"L00000000032$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$6, %ecx
	movl	-36(%ebp), %edx
	andl	$16777215, %edx
	leal	L__register$non_lazy_ptr-"L00000000032$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000032$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$4, %ecx
	movl	-12(%ebp), %edx
	andl	$16777215, %edx
	leal	L__register$non_lazy_ptr-"L00000000032$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	leal	L_iselect$non_lazy_ptr-"L00000000032$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	orl	$5, %ecx
	movl	-40(%ebp), %eax
	movl	%eax, %edx
	andl	$16777215, %edx
	leal	L__register$non_lazy_ptr-"L00000000032$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%ecx,4)
	addl	$52, %esp
	popl	%ebx
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
