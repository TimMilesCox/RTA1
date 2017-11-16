	.data
	.align 2
_attention:
	.long	0
	.word	1
	.space 2
.globl _iselect
	.align 2
_iselect:
	.long	128
.globl _apc
	.align 2
_apc:
	.long	_memory
.globl _b0p
	.align 2
_b0p:
	.long	_memory
.globl _psr
	.align 2
_psr:
	.long	8388608
.globl __register
	.align 5
__register:
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	10855845
	.long	5921370
	.long	10855845
	.long	5921370
	.long	10855845
	.long	5921370
	.long	10855845
	.long	5921370
	.long	10855845
	.long	5921370
	.long	10855845
	.long	5921370
	.long	10855845
	.long	5921370
	.space 552
.globl _register_set
	.align 2
_register_set:
	.long	__register+512
	.cstring
LC0:
	.ascii "rom image file not available\0"
LC1:
	.ascii "reading %s with options:\12\0"
LC2:
	.ascii "-s\11single step\0"
LC3:
	.ascii "-v\11vocal\0"
LC4:
	.ascii "-l\11line per instruction\0"
LC5:
	.ascii "last load word incomplete\0"
	.align 2
LC6:
	.ascii "rom image %d target words read\12\0"
LC7:
	.ascii "rom image file required\0"
LC8:
	.ascii "threadcbinit %d e %d\12\0"
LC9:
	.ascii "async thread start %d %d\12\0"
LC10:
	.ascii "async thread ID %p\12\0"
LC11:
	.ascii "g[break:point] to run\0"
LC12:
	.ascii "s to enter single step\0"
LC13:
	.ascii "key %s\12\12\0"
	.align 2
LC14:
	.ascii "problem with input request %d. exit emulator\12\0"
LC15:
	.ascii "stdin terminated\0"
	.text
.globl _main
_main:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	pushl	%ebx
	subl	$208, %esp
	call	L39
"L00000000001$pb":
L39:
	popl	%ebx
	movl	12(%ebp), %eax
	movl	%eax, -172(%ebp)
	leal	L___stack_chk_guard$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %edx
	movl	%edx, -12(%ebp)
	xorl	%edx, %edx
	movl	$0, 4(%esp)
	leal	_time1-"L00000000001$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_gettimeofday
	leal	_time1-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$1000, %eax, %esi
	leal	_time1-"L00000000001$pb"(%ebx), %eax
	movl	4(%eax), %ecx
	movl	$274877907, -188(%ebp)
	movl	-188(%ebp), %eax
	imull	%ecx
	sarl	$6, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	movl	%edx, %ecx
	subl	%eax, %ecx
	movl	%ecx, %eax
	leal	(%esi,%eax), %eax
	movl	%eax, %edx
	sarl	$31, %edx
	leal	_start_time-"L00000000001$pb"(%ebx), %ecx
	movl	%eax, (%ecx)
	movl	%edx, 4(%ecx)
	movl	$0, -148(%ebp)
	leal	L_memory$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, -152(%ebp)
	movb	$0, -156(%ebp)
	movb	$0, -155(%ebp)
	movb	$0, -154(%ebp)
	movb	$0, -153(%ebp)
	movl	-172(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_argue
	leal	L_arguments$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	testl	%eax, %eax
	je	L2
	leal	L_argument$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	$292, 8(%esp)
	movl	$0, 4(%esp)
	movl	%eax, (%esp)
	call	_open$UNIX2003
	movl	%eax, -140(%ebp)
	cmpl	$0, -140(%ebp)
	jns	L4
	leal	LC0-"L00000000001$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	movl	$0, -180(%ebp)
	jmp	L6
L4:
	leal	L_flag$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	21(%eax), %eax
	testb	%al, %al
	je	L7
	leal	L_argument$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, 4(%esp)
	leal	LC1-"L00000000001$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	leal	L_flag$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	18(%eax), %eax
	testb	%al, %al
	je	L9
	leal	LC2-"L00000000001$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
L9:
	leal	L_flag$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	21(%eax), %eax
	testb	%al, %al
	je	L11
	leal	LC3-"L00000000001$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
L11:
	leal	L_flag$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	11(%eax), %eax
	testb	%al, %al
	je	L7
	leal	LC4-"L00000000001$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
L7:
	movl	$3, 8(%esp)
	leal	-156(%ebp), %eax
	incl	%eax
	movl	%eax, 4(%esp)
	movl	-140(%ebp), %eax
	movl	%eax, (%esp)
	call	_read
	movl	%eax, -144(%ebp)
	cmpl	$0, -144(%ebp)
	jle	L14
	cmpl	$3, -144(%ebp)
	je	L16
	leal	LC5-"L00000000001$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
L16:
	movl	-152(%ebp), %edx
	movl	-156(%ebp), %eax
	movl	%eax, (%edx)
	addl	$4, -152(%ebp)
	incl	-148(%ebp)
	jmp	L7
L14:
	movl	-148(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC6-"L00000000001$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	addl	$4095, -148(%ebp)
	movl	-148(%ebp), %eax
	sarl	$12, %eax
	movl	%eax, %edx
	leal	L_base$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, 496(%eax)
	leal	L_base$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	$127, 512(%eax)
	movl	-140(%ebp), %eax
	movl	%eax, (%esp)
	call	_close
	call	_netbank
	leal	L_arguments$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	cmpl	$1, %eax
	jg	L18
	jmp	L19
L2:
	leal	LC7-"L00000000001$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	movl	$0, -180(%ebp)
	jmp	L6
L18:
	leal	L_argument$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	4(%eax), %eax
	movl	%eax, (%esp)
	call	_load_fs
L19:
	leal	-52(%ebp), %eax
	movl	%eax, (%esp)
	call	_pthread_attr_init
	movl	%eax, -128(%ebp)
	cmpl	$0, -128(%ebp)
	jns	L20
	call	___error
	movl	(%eax), %eax
	movl	%eax, 8(%esp)
	movl	-128(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC8-"L00000000001$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	jmp	L22
L20:
	movl	$0, 12(%esp)
	leal	_emulate-"L00000000001$pb"(%ebx), %eax
	movl	%eax, 8(%esp)
	leal	-52(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	-136(%ebp), %eax
	movl	%eax, (%esp)
	call	_pthread_create
	movl	%eax, -128(%ebp)
	cmpl	$0, -128(%ebp)
	jns	L23
	call	___error
	movl	(%eax), %eax
	movl	%eax, 8(%esp)
	movl	-128(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC9-"L00000000001$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	jmp	L22
L23:
	movl	-136(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC10-"L00000000001$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L22:
	leal	L_flag$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	18(%eax), %eax
	testb	%al, %al
	je	L25
	leal	LC11-"L00000000001$pb"(%ebx), %eax
	movl	%eax, -176(%ebp)
	jmp	L27
L25:
	leal	LC12-"L00000000001$pb"(%ebx), %edx
	movl	%edx, -176(%ebp)
L27:
	movl	-176(%ebp), %ecx
	movl	%ecx, 4(%esp)
	leal	LC13-"L00000000001$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L28:
	movl	$1000, 8(%esp)
	movl	$1, 4(%esp)
	leal	_attention-"L00000000001$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_poll$UNIX2003
	movl	%eax, -128(%ebp)
	cmpl	$0, -128(%ebp)
	jns	L29
	call	___error
	movl	(%eax), %eax
	movl	%eax, 4(%esp)
	leal	LC14-"L00000000001$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	jmp	L31
L29:
	cmpl	$0, -128(%ebp)
	je	L32
	leal	L_flag$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movb	$1, 18(%eax)
	movl	$62, (%esp)
	call	_putchar
	leal	L___stdoutp$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	_fflush
	leal	L___stdinp$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, 8(%esp)
	movl	$71, 4(%esp)
	leal	-124(%ebp), %eax
	movl	%eax, (%esp)
	call	_fgets
	movl	%eax, -132(%ebp)
	cmpl	$0, -132(%ebp)
	jne	L34
	leal	LC15-"L00000000001$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	jmp	L31
L34:
	movl	-132(%ebp), %eax
	movzbl	(%eax), %eax
	cmpb	$46, %al
	je	L31
	movl	-132(%ebp), %eax
	movl	%eax, (%esp)
	call	_action
L32:
	movl	$0, 4(%esp)
	leal	_time1-"L00000000001$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_gettimeofday
	leal	_time1-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$1000, %eax, %esi
	leal	_time1-"L00000000001$pb"(%ebx), %eax
	movl	4(%eax), %ecx
	movl	$274877907, -188(%ebp)
	movl	-188(%ebp), %eax
	imull	%ecx
	sarl	$6, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	movl	%edx, %ecx
	subl	%eax, %ecx
	movl	%ecx, %eax
	leal	(%esi,%eax), %eax
	movl	%eax, %edx
	sarl	$31, %edx
	leal	_u-"L00000000001$pb"(%ebx), %ecx
	movl	%eax, (%ecx)
	movl	%edx, 4(%ecx)
	leal	_indication-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$1, %edx
	leal	_indication-"L00000000001$pb"(%ebx), %eax
	movl	%edx, (%eax)
	jmp	L28
L31:
	movl	$0, -180(%ebp)
L6:
	movl	-180(%ebp), %eax
	leal	L___stack_chk_guard$non_lazy_ptr-"L00000000001$pb"(%ebx), %edx
	movl	(%edx), %edx
	movl	-12(%ebp), %ecx
	xorl	(%edx), %ecx
	je	L38
	call	___stack_chk_fail
L38:
	addl	$208, %esp
	popl	%ebx
	popl	%esi
	leave
	ret
	.cstring
LC16:
	.ascii "emulation start\0"
LC17:
	.ascii "[%x %p %x %p]\12\0"
	.align 2
LC18:
	.ascii "[0:%8.8x 1:%8.8x 2:%8.8x 3:%8.8x]\12\0"
	.text
_emulate:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$60, %esp
	call	L60
"L00000000002$pb":
L60:
	popl	%ebx
	leal	LC16-"L00000000002$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
L41:
	leal	L_flag$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	18(%eax), %eax
	testb	%al, %al
	je	L42
	leal	_indication-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$2, %edx
	leal	_indication-"L00000000002$pb"(%ebx), %eax
	movl	%edx, (%eax)
L42:
	leal	L_flag$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	4(%eax), %eax
	testb	%al, %al
	je	L44
	leal	_apc-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %ecx
	leal	_psr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %esi
	leal	_register_set-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %edx
	leal	_indication-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%ecx, 16(%esp)
	movl	%esi, 12(%esp)
	movl	%edx, 8(%esp)
	movl	%eax, 4(%esp)
	leal	LC17-"L00000000002$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L44:
	L46: # LASM$engage
	# top of block
	.file "tipt.c"
	.line 303
	pushl %ebp
	.file "tipt.c"
	.line 304
	movl %esi, %ebp
	.file "tipt.c"
	.line 305
	pushl %ebp
	.file "tipt.c"
	.line 310
	pushl %edi
	.file "tipt.c"
	.line 311
	pushl %edx
	.file "tipt.c"
	.line 312
	pushl %ecx
	.file "tipt.c"
	.line 313
	pushl %ebx
	.file "tipt.c"
	.line 314
	pushl %eax
	.file "tipt.c"
	.line 319
	movl _register_set, %ebp
	.file "tipt.c"
	.line 320
	movl _apc, %edx
	L47: # LASM$next
	.file "tipt.c"
	.line 321
	movl 0(%edx), %eax
	.file "tipt.c"
	.line 322
	addl $4, %edx
	.file "tipt.c"
	.line 323
	bswapl %eax
	.file "tipt.c"
	.line 324
	call _execute
	.file "tipt.c"
	.line 325
	testl $7, _indication
	.file "tipt.c"
	.line 326
	jz L47
	.file "tipt.c"
	.line 327
	movl %ebp, _register_set
	.file "tipt.c"
	.line 328
	movl %edx, _apc
	.file "tipt.c"
	.line 330
	popl %eax
	.file "tipt.c"
	.line 331
	popl %ebx
	.file "tipt.c"
	.line 332
	popl %ecx
	.file "tipt.c"
	.line 333
	popl %edx
	.file "tipt.c"
	.line 334
	popl %edi
	.file "tipt.c"
	.line 337
	popl %ebp
	.file "tipt.c"
	.line 338
	movl %ebp, %esi
	.file "tipt.c"
	.line 339
	popl %ebp
	leal	L_flag$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	4(%eax), %eax
	testb	%al, %al
	je	L48
	leal	L_trace3$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %edx
	leal	L_trace2$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %ecx
	leal	L_trace1$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %esi
	leal	L_trace$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%edx, 16(%esp)
	movl	%ecx, 12(%esp)
	movl	%esi, 8(%esp)
	movl	%eax, 4(%esp)
	leal	LC18-"L00000000002$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L48:
	leal	_indication-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	andl	$1, %eax
	testb	%al, %al
	je	L50
	leal	_u-"L00000000002$pb"(%ebx), %eax
	movl	4(%eax), %edx
	movl	(%eax), %eax
	andl	$16777215, %eax
	movl	%eax, -28(%ebp)
	leal	_u-"L00000000002$pb"(%ebx), %eax
	movl	4(%eax), %edx
	movl	(%eax), %eax
	shrdl	$24, %edx, %eax
	sarl	$24, %edx
	andl	$16777215, %eax
	movl	%eax, -32(%ebp)
	leal	_u-"L00000000002$pb"(%ebx), %eax
	movl	4(%eax), %edx
	movl	(%eax), %eax
	movl	%eax, %edx
	andl	$16777215, %edx
	leal	__register-"L00000000002$pb"(%ebx), %eax
	movl	%edx, 604(%eax)
	leal	_u-"L00000000002$pb"(%ebx), %eax
	movl	4(%eax), %edx
	movl	(%eax), %eax
	shrdl	$24, %edx, %eax
	sarl	$24, %edx
	movl	%eax, %edx
	andl	$16777215, %edx
	leal	__register-"L00000000002$pb"(%ebx), %eax
	movl	%edx, 600(%eax)
	leal	_indication-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	andl	$-2, %edx
	leal	_indication-"L00000000002$pb"(%ebx), %eax
	movl	%edx, (%eax)
	leal	_indication-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	testl	%eax, %eax
	je	L41
L50:
	leal	_indication-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	andl	$4, %eax
	testl	%eax, %eax
	je	L53
	leal	_apc-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %edx
	leal	_breakpoint-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	cmpl	%eax, %edx
	jne	L53
	leal	L_flag$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movb	$1, 18(%eax)
	leal	_indication-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$2, %edx
	leal	_indication-"L00000000002$pb"(%ebx), %eax
	movl	%edx, (%eax)
L53:
	leal	_indication-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	andl	$2, %eax
	testl	%eax, %eax
	je	L41
	call	_statement
	movl	$62, (%esp)
	call	_putchar
	leal	L___stdoutp$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	_fflush
	jmp	L57
L58:
	movl	$10000, (%esp)
	call	_usleep
L57:
	leal	_indication-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	andl	$2, %eax
	testl	%eax, %eax
	jne	L58
	jmp	L41
	.cstring
LC19:
	.ascii "key s for single step\0"
LC20:
	.ascii "%x:%x\0"
LC21:
	.ascii "[>%x,%x]\0"
LC22:
	.ascii "[@%p]\0"
LC23:
	.ascii "%x\0"
LC24:
	.ascii "%d\0"
LC25:
	.ascii "%6.6x :\0"
LC26:
	.ascii " %2.2x%2.2x%2.2x\0"
LC27:
	.ascii "%6.6x:%6.6x :\0"
LC28:
	.ascii " %6.6x\0"
LC29:
	.ascii "\12%2.2x:\0"
	.align 2
LC30:
	.ascii "\12"
	.ascii "7c                             \0"
LC31:
	.ascii "%lld\0"
LC32:
	.ascii "%s\0"
	.align 2
LC33:
	.ascii "LF\11execute one instruction and display new state\0"
	.align 2
LC34:
	.ascii "+\11display more internal stack registers\0"
	.align 2
LC35:
	.ascii "0n\11display registers starting at hex address\0"
	.align 2
LC36:
	.ascii "r n\11display registers starting at decimal address\0"
	.align 2
LC37:
	.ascii "b\11display relocation / configuration ports\12\0"
	.align 2
LC38:
	.ascii "m [[page:]offset]\11display system memory\0"
	.align 2
LC39:
	.ascii "\11default page[s] in current address space\12\0"
	.align 2
LC40:
	.ascii "d [[device:]offset]\11display peripheral memory array[s]\0"
	.align 2
LC41:
	.ascii "\11default device[s] in current address space\12\0"
	.align 2
LC42:
	.ascii "g [[page:]breakpoint]\11run [to breakpoint]\0"
	.align 2
LC43:
	.ascii "\11default page is current instruction section\0"
LC44:
	.ascii "\11to remove breakpoint g0:0\12\0"
LC45:
	.ascii ".\11exit emulator\0"
	.text
_action:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$436, %esp
	call	L142
"L00000000003$pb":
L142:
	popl	%ebx
	movl	8(%ebp), %eax
	movl	%eax, -412(%ebp)
	leal	L___stack_chk_guard$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %edx
	movl	%edx, -12(%ebp)
	xorl	%edx, %edx
	movl	-412(%ebp), %eax
	movzbl	(%eax), %eax
	movsbl	%al,%eax
	movl	%eax, -408(%ebp)
	leal	L_flag$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	18(%eax), %eax
	testb	%al, %al
	jne	L62
	cmpl	$115, -408(%ebp)
	je	L62
	leal	LC19-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	jmp	L140
L62:
	movl	-408(%ebp), %ecx
	subl	$43, %ecx
	movl	%ecx, -416(%ebp)
	cmpl	$79, -416(%ebp)
	ja	L66
	movl	-416(%ebp), %edx
	sall	$2, %edx
	leal	L78-"L00000000003$pb"(%ebx), %eax
	movl	(%edx,%eax), %eax
	addl	%ebx, %eax
	jmp	*%eax
	.align 2,0x90
L78:
	.long	L67-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L68-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L69-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L71-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L72-"L00000000003$pb"
	.long	L73-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L74-"L00000000003$pb"
	.long	L75-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L69-"L00000000003$pb"
	.long	L76-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L77-"L00000000003$pb"
L76:
	leal	_indication-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$2, %edx
	leal	_indication-"L00000000003$pb"(%ebx), %eax
	movl	%edx, (%eax)
	leal	L_flag$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movb	$1, 18(%eax)
	jmp	L140
L72:
	movl	-412(%ebp), %edx
	incl	%edx
	leal	-400(%ebp), %eax
	movl	%eax, 12(%esp)
	leal	-388(%ebp), %eax
	movl	%eax, 8(%esp)
	leal	LC20-"L00000000003$pb"(%ebx), %eax
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	_sscanf
	movl	%eax, -384(%ebp)
	cmpl	$1, -384(%ebp)
	jne	L79
	leal	_b0p-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	movl	-388(%ebp), %eax
	sall	$2, %eax
	addl	%eax, %edx
	leal	_breakpoint-"L00000000003$pb"(%ebx), %eax
	movl	%edx, (%eax)
L79:
	cmpl	$2, -384(%ebp)
	jne	L81
	movl	-388(%ebp), %eax
	movl	%eax, %edx
	sall	$14, %edx
	leal	L_memory$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	addl	%eax, %edx
	movl	-400(%ebp), %eax
	sall	$2, %eax
	addl	%eax, %edx
	leal	_breakpoint-"L00000000003$pb"(%ebx), %eax
	movl	%edx, (%eax)
L81:
	leal	L_flag$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movb	$0, 18(%eax)
	leal	_indication-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	andl	$-3, %edx
	leal	_indication-"L00000000003$pb"(%ebx), %eax
	movl	%edx, (%eax)
	leal	L_flag$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	21(%eax), %eax
	testb	%al, %al
	je	L83
	leal	_breakpoint-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	leal	L_memory$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, %ecx
	subl	%eax, %ecx
	movl	%ecx, %eax
	sarl	$2, %eax
	movl	%eax, %edx
	leal	_indication-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, 8(%esp)
	movl	%eax, 4(%esp)
	leal	LC21-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L83:
	leal	L_flag$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	4(%eax), %eax
	testb	%al, %al
	je	L85
	leal	_breakpoint-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, 4(%esp)
	leal	LC22-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L85:
	movl	$58, (%esp)
	call	_putchar
	leal	L___stdoutp$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	_fflush
	jmp	L140
L67:
	leal	_iselect-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$15, %edx
	leal	__register-"L00000000003$pb"(%ebx), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, -388(%ebp)
	movl	-388(%ebp), %eax
	addl	$23, %eax
	movl	%eax, -388(%ebp)
L69:
	cmpl	$43, -408(%ebp)
	je	L87
	leal	_iselect-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	orl	$24, %eax
	movl	%eax, -388(%ebp)
L87:
	cmpl	$48, -408(%ebp)
	jne	L89
	leal	-388(%ebp), %eax
	movl	%eax, 8(%esp)
	leal	LC23-"L00000000003$pb"(%ebx), %eax
	movl	%eax, 4(%esp)
	movl	-412(%ebp), %eax
	movl	%eax, (%esp)
	call	_sscanf
	jmp	L93
L89:
	cmpl	$114, -408(%ebp)
	jne	L93
	movl	-412(%ebp), %eax
	incl	%eax
	leal	-388(%ebp), %edx
	movl	%edx, 8(%esp)
	leal	LC24-"L00000000003$pb"(%ebx), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	_sscanf
	jmp	L93
L94:
	movl	-388(%ebp), %eax
	movl	%eax, (%esp)
	call	_print_register_row
	movl	-388(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -388(%ebp)
	leal	L___stdinp$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, 8(%esp)
	movl	$48, 4(%esp)
	movl	-412(%ebp), %eax
	movl	%eax, (%esp)
	call	_fgets
	movl	-412(%ebp), %eax
	movzbl	(%eax), %eax
	cmpb	$46, %al
	je	L140
L93:
	movl	-388(%ebp), %eax
	cmpl	$255, %eax
	jle	L94
	jmp	L140
L75:
	movl	-412(%ebp), %edx
	incl	%edx
	leal	-400(%ebp), %eax
	movl	%eax, 12(%esp)
	leal	-396(%ebp), %eax
	movl	%eax, 8(%esp)
	leal	LC20-"L00000000003$pb"(%ebx), %eax
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	_sscanf
	movl	%eax, -384(%ebp)
	cmpl	$1, -384(%ebp)
	jg	L96
	movl	-396(%ebp), %eax
	movl	%eax, -400(%ebp)
L98:
	movl	-400(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -388(%ebp)
	movl	-400(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC25-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	jmp	L99
L100:
	movl	-400(%ebp), %eax
	movl	%eax, %edx
	incl	%eax
	movl	%eax, -400(%ebp)
	movl	%edx, (%esp)
	call	_memory_read
	movl	%eax, -376(%ebp)
	movzbl	-373(%ebp), %eax
	movzbl	%al, %edx
	movzbl	-374(%ebp), %eax
	movzbl	%al, %ecx
	movzbl	-375(%ebp), %eax
	movzbl	%al, %eax
	movl	%edx, 12(%esp)
	movl	%ecx, 8(%esp)
	movl	%eax, 4(%esp)
	leal	LC26-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L99:
	movl	-400(%ebp), %edx
	movl	-388(%ebp), %eax
	cmpl	%eax, %edx
	jl	L100
	leal	L___stdinp$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, 8(%esp)
	movl	$48, 4(%esp)
	movl	-412(%ebp), %eax
	movl	%eax, (%esp)
	call	_fgets
	movl	-412(%ebp), %eax
	movzbl	(%eax), %eax
	cmpb	$46, %al
	je	L140
	jmp	L98
L96:
	movl	-396(%ebp), %eax
	movl	%eax, %edx
	sall	$12, %edx
	movl	-400(%ebp), %eax
	leal	(%edx,%eax), %eax
	movl	%eax, -404(%ebp)
L105:
	movl	-404(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -388(%ebp)
	movl	-400(%ebp), %eax
	movl	-396(%ebp), %edx
	movl	%eax, 8(%esp)
	movl	%edx, 4(%esp)
	leal	LC27-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	movl	-400(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -400(%ebp)
	jmp	L106
L107:
	movl	-404(%ebp), %eax
	cmpl	$524287, %eax
	jg	L108
	movl	-404(%ebp), %edx
	leal	L_memory$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, -376(%ebp)
	leal	1(%edx), %eax
	movl	%eax, -404(%ebp)
	movzbl	-373(%ebp), %eax
	movzbl	%al, %edx
	movzbl	-374(%ebp), %eax
	movzbl	%al, %ecx
	movzbl	-375(%ebp), %eax
	movzbl	%al, %eax
	movl	%edx, 12(%esp)
	movl	%ecx, 8(%esp)
	movl	%eax, 4(%esp)
	leal	LC26-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L106:
	movl	-404(%ebp), %edx
	movl	-388(%ebp), %eax
	cmpl	%eax, %edx
	jl	L107
L108:
	leal	L___stdinp$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, 8(%esp)
	movl	$48, 4(%esp)
	movl	-412(%ebp), %eax
	movl	%eax, (%esp)
	call	_fgets
	movl	-412(%ebp), %eax
	movzbl	(%eax), %eax
	cmpb	$46, %al
	je	L140
	jmp	L105
L71:
	movl	$0, -404(%ebp)
	movl	$0, -400(%ebp)
	movl	$0, -392(%ebp)
	movl	-412(%ebp), %edx
	incl	%edx
	leal	-400(%ebp), %eax
	movl	%eax, 12(%esp)
	leal	-404(%ebp), %eax
	movl	%eax, 8(%esp)
	leal	LC20-"L00000000003$pb"(%ebx), %eax
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	_sscanf
	movl	%eax, -384(%ebp)
	cmpl	$1, -384(%ebp)
	jg	L111
	movl	-404(%ebp), %eax
	movl	%eax, -400(%ebp)
	movl	$0, -404(%ebp)
	movl	-400(%ebp), %eax
	andl	$16515072, %eax
	movl	%eax, -392(%ebp)
	cmpl	$0, -392(%ebp)
	je	L113
	sarl	$18, -392(%ebp)
	movl	-400(%ebp), %eax
	andl	$262143, %eax
	movl	%eax, -400(%ebp)
	jmp	L115
L113:
	movl	-400(%ebp), %eax
	andl	$258048, %eax
	movl	%eax, -392(%ebp)
	sarl	$12, -392(%ebp)
	movl	-400(%ebp), %eax
	andl	$4095, %eax
	movl	%eax, -400(%ebp)
L115:
	cmpl	$0, -392(%ebp)
	je	L116
	cmpl	$7, -392(%ebp)
	jg	L116
	movl	-392(%ebp), %ecx
	movl	$32768, %eax
	sarl	%cl, %eax
	movl	%eax, %edx
	leal	_psr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	andl	%edx, %eax
	testl	%eax, %eax
	je	L116
	addl	$64, -392(%ebp)
L116:
	movl	-392(%ebp), %edx
	leal	L_base$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, -396(%ebp)
	movl	-396(%ebp), %eax
	andl	$4194304, %eax
	testl	%eax, %eax
	je	L122
	movl	-396(%ebp), %eax
	andl	$63, %eax
	movl	%eax, -404(%ebp)
	jmp	L122
L111:
	movl	-404(%ebp), %eax
	orl	$4194304, %eax
	movl	%eax, -396(%ebp)
L122:
	movl	-400(%ebp), %eax
	movl	-396(%ebp), %edx
	movl	%eax, 8(%esp)
	movl	%edx, 4(%esp)
	leal	LC27-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	movl	-400(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -388(%ebp)
	jmp	L123
L124:
	movl	-400(%ebp), %eax
	movl	%eax, %ecx
	incl	%eax
	movl	%eax, -400(%ebp)
	movl	-396(%ebp), %eax
	movl	-404(%ebp), %edx
	movl	%ecx, 8(%esp)
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	_device_read
	movl	%eax, -384(%ebp)
	movl	-384(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC28-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L123:
	movl	-400(%ebp), %edx
	movl	-388(%ebp), %eax
	cmpl	%eax, %edx
	jl	L124
	leal	L___stdinp$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, 8(%esp)
	movl	$48, 4(%esp)
	movl	-412(%ebp), %eax
	movl	%eax, (%esp)
	call	_fgets
	movl	-412(%ebp), %eax
	movzbl	(%eax), %eax
	cmpb	$46, %al
	je	L140
	jmp	L122
L70:
	movl	$0, -384(%ebp)
	jmp	L128
L129:
	movl	-384(%ebp), %eax
	andl	$7, %eax
	testl	%eax, %eax
	jne	L130
	movl	-384(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC29-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L130:
	movl	-384(%ebp), %edx
	leal	L_base$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, 4(%esp)
	leal	LC28-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	incl	-384(%ebp)
L128:
	cmpl	$71, -384(%ebp)
	jle	L129
	leal	LC30-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	movl	$124, -384(%ebp)
	jmp	L133
L134:
	movl	-384(%ebp), %eax
	andl	$7, %eax
	testl	%eax, %eax
	jne	L135
	movl	-384(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC29-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L135:
	movl	-384(%ebp), %edx
	leal	L_base$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, 4(%esp)
	leal	LC28-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	incl	-384(%ebp)
L133:
	cmpl	$191, -384(%ebp)
	jle	L134
	movl	$10, (%esp)
	call	_putchar
	jmp	L140
L77:
	movl	-412(%ebp), %eax
	incl	%eax
	leal	_start_time-"L00000000003$pb"(%ebx), %edx
	movl	%edx, 8(%esp)
	leal	LC31-"L00000000003$pb"(%ebx), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	_sscanf
	jmp	L140
L74:
	movl	-412(%ebp), %edx
	incl	%edx
	leal	-372(%ebp), %eax
	movl	%eax, 8(%esp)
	leal	LC32-"L00000000003$pb"(%ebx), %eax
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	_sscanf
	movl	%eax, -384(%ebp)
	cmpl	$1, -384(%ebp)
	jne	L140
	leal	-372(%ebp), %eax
	movl	%eax, (%esp)
	call	_load_fs
	jmp	L140
L73:
	leal	LC33-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	leal	LC34-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	leal	LC35-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	leal	LC36-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	leal	LC37-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	leal	LC38-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	leal	LC39-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	leal	LC40-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	leal	LC41-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	leal	LC42-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	leal	LC43-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	leal	LC44-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	leal	LC45-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	jmp	L140
L68:
	leal	_indication-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	andl	$-3, %edx
	leal	_indication-"L00000000003$pb"(%ebx), %eax
	movl	%edx, (%eax)
	jmp	L140
L66:
	leal	_indication-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	andl	$-3, %edx
	leal	_indication-"L00000000003$pb"(%ebx), %eax
	movl	%edx, (%eax)
L140:
	leal	L___stack_chk_guard$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	-12(%ebp), %edx
	xorl	(%eax), %edx
	je	L141
	call	___stack_chk_fail
L141:
	addl	$436, %esp
	popl	%ebx
	leave
	ret
	.cstring
	.align 2
LC46:
	.ascii "file system image %s error %d\12\0"
LC47:
	.ascii "loading file system image %s\12\0"
	.align 2
LC48:
	.ascii "volume header read unsuccessful\0"
LC49:
	.ascii "[%2.2x%2.2x%2.2x]\12\0"
LC50:
	.ascii "\12%4.4x: \0"
LC51:
	.ascii "%2.2x\0"
LC52:
	.ascii "%.*s %d storage banks\12\0"
	.align 2
LC53:
	.ascii "%d unable to cache file system\12\0"
LC54:
	.ascii "%d granules loaded\12\0"
	.text
_load_fs:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$3124, %esp
	call	L173
"L00000000004$pb":
L173:
	popl	%ebx
	movl	$0, -20(%ebp)
	leal	L_devices$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	12(%eax), %eax
	movl	%eax, -24(%ebp)
	movl	$292, 8(%esp)
	movl	$0, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_open$UNIX2003
	movl	%eax, -28(%ebp)
	cmpl	$0, -28(%ebp)
	jns	L144
	call	___error
	movl	(%eax), %eax
	movl	%eax, 8(%esp)
	movl	8(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC46-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	jmp	L172
L144:
	movl	8(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC47-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	movl	$3072, 8(%esp)
	leal	-3100(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	-28(%ebp), %eax
	movl	%eax, (%esp)
	call	_read
	movl	%eax, -12(%ebp)
	cmpl	$3072, -12(%ebp)
	je	L147
	leal	LC48-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	jmp	L172
L147:
	leal	L_flag$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	21(%eax), %eax
	testb	%al, %al
	je	L149
	movzbl	-3053(%ebp), %eax
	movzbl	%al, %edx
	movzbl	-3054(%ebp), %eax
	movzbl	%al, %ecx
	movzbl	-3055(%ebp), %eax
	movzbl	%al, %eax
	movl	%edx, 12(%esp)
	movl	%ecx, 8(%esp)
	movl	%eax, 4(%esp)
	leal	LC49-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L149:
	movzbl	-3055(%ebp), %eax
	cmpb	$86, %al
	je	L151
	leal	L_flag$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	21(%eax), %eax
	testb	%al, %al
	je	L172
	movl	$0, -20(%ebp)
	leal	-3100(%ebp), %eax
	movl	%eax, -24(%ebp)
	jmp	L155
L156:
	movl	-20(%ebp), %eax
	andl	$15, %eax
	testl	%eax, %eax
	jne	L157
	movl	-20(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC50-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L157:
	movl	-24(%ebp), %eax
	movzbl	(%eax), %eax
	movsbl	%al,%eax
	incl	-24(%ebp)
	movl	%eax, 4(%esp)
	leal	LC51-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	incl	-20(%ebp)
L155:
	decl	-12(%ebp)
	cmpl	$-1, -12(%ebp)
	jne	L156
	movl	$10, (%esp)
	call	_putchar
	jmp	L172
L151:
	movzbl	-3053(%ebp), %eax
	movzbl	%al, %edx
	movl	%edx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	subl	$9, %eax
	movl	%eax, -12(%ebp)
	movzwl	-3052(%ebp), %eax
	movw	%ax, (%esp)
	movzbl	-3050(%ebp), %eax
	movb	%al, 2(%esp)
	call	_msw2i
	movl	%eax, -16(%ebp)
	movl	-16(%ebp), %eax
	movl	%eax, 12(%esp)
	leal	-3100(%ebp), %eax
	addl	$57, %eax
	movl	%eax, 8(%esp)
	movl	-12(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC52-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	cmpl	$0, -16(%ebp)
	je	L172
	cmpl	$0, -24(%ebp)
	je	L161
	movl	-24(%ebp), %eax
	movl	%eax, (%esp)
	call	_free
L161:
	movl	-16(%ebp), %eax
	movl	$786432, 4(%esp)
	movl	%eax, (%esp)
	call	_calloc
	movl	%eax, -24(%ebp)
	cmpl	$0, -24(%ebp)
	jne	L163
	call	___error
	movl	(%eax), %eax
	movl	%eax, 4(%esp)
	leal	LC53-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	jmp	L172
L163:
	movl	-24(%ebp), %edx
	leal	L_devices$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, 12(%eax)
	movl	-16(%ebp), %eax
	orl	$14680064, %eax
	movl	%eax, %edx
	leal	L_base$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, 516(%eax)
	movl	$-1, %eax
	cmpl	$-1, %eax
	je	L165
	movl	$-1, %eax
	movl	%eax, 12(%esp)
	movl	$3072, 8(%esp)
	leal	-3100(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	-24(%ebp), %eax
	movl	%eax, (%esp)
	call	___memcpy_chk
	jmp	L167
L165:
	movl	$3072, 8(%esp)
	leal	-3100(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	-24(%ebp), %eax
	movl	%eax, (%esp)
	call	___inline_memcpy_chk
L167:
	addl	$3072, -24(%ebp)
	movl	$16, -20(%ebp)
L168:
	movl	$192, 8(%esp)
	movl	-24(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	-28(%ebp), %eax
	movl	%eax, (%esp)
	call	_read
	movl	%eax, -12(%ebp)
	cmpl	$0, -12(%ebp)
	js	L169
	cmpl	$0, -12(%ebp)
	je	L169
	incl	-20(%ebp)
	addl	$192, -24(%ebp)
	jmp	L168
L169:
	movl	-28(%ebp), %eax
	movl	%eax, (%esp)
	call	_close
	movl	-20(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC54-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L172:
	addl	$3124, %esp
	popl	%ebx
	leave
	ret
___inline_memcpy_chk:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	movl	$-1, %eax
	movl	%eax, 12(%esp)
	movl	16(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	___memcpy_chk
	leave
	ret
	.cstring
LC55:
	.ascii "[RP %p]\0"
LC56:
	.ascii "%6.6x %8.8x\12\0"
LC57:
	.ascii "[%6.6x]->\0"
LC58:
	.ascii "\12   \0"
	.text
_statement:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$36, %esp
	call	L195
"L00000000005$pb":
L195:
	popl	%ebx
	leal	_iselect-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, -12(%ebp)
	leal	L_flag$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	11(%eax), %eax
	movsbl	%al,%edx
	leal	_apc-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	subl	$4, %eax
	movl	%edx, 8(%esp)
	movl	$1, 4(%esp)
	movl	%eax, (%esp)
	call	_instruction_display
	leal	L_flag$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	4(%eax), %eax
	testb	%al, %al
	je	L177
	leal	_register_set-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, 4(%esp)
	leal	LC55-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L177:
	leal	_apc-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	leal	L_memory$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, %ecx
	subl	%eax, %ecx
	movl	%ecx, %eax
	sarl	$2, %eax
	movl	%eax, %edx
	leal	_psr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, 8(%esp)
	movl	%eax, 4(%esp)
	leal	LC56-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	leal	L_flag$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	11(%eax), %eax
	movsbl	%al,%edx
	leal	_apc-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, 8(%esp)
	movl	$6, 4(%esp)
	movl	%eax, (%esp)
	call	_instruction_display
	jmp	L179
L180:
	movl	-12(%ebp), %eax
	movl	%eax, (%esp)
	call	_print_register_row
	movl	$10, (%esp)
	call	_putchar
	addl	$8, -12(%ebp)
L179:
	leal	_iselect-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	orl	$24, %eax
	cmpl	-12(%ebp), %eax
	jg	L180
	leal	_iselect-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$15, %edx
	leal	__register-"L00000000005$pb"(%ebx), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC57-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	movl	$7, -16(%ebp)
	jmp	L182
L183:
	cmpl	$255, -12(%ebp)
	jg	L184
	movl	-12(%ebp), %edx
	leal	__register-"L00000000005$pb"(%ebx), %eax
	movl	(%eax,%edx,4), %eax
	incl	-12(%ebp)
	movl	%eax, 4(%esp)
	leal	LC28-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L182:
	decl	-16(%ebp)
	cmpl	$-1, -16(%ebp)
	jne	L183
L184:
	leal	LC58-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	movl	$8, -16(%ebp)
	jmp	L186
L187:
	cmpl	$255, -12(%ebp)
	jg	L188
	movl	-12(%ebp), %edx
	leal	__register-"L00000000005$pb"(%ebx), %eax
	movl	(%eax,%edx,4), %eax
	incl	-12(%ebp)
	movl	%eax, 4(%esp)
	leal	LC28-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L186:
	decl	-16(%ebp)
	cmpl	$-1, -16(%ebp)
	jne	L187
L188:
	leal	LC58-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	movl	$8, -16(%ebp)
	jmp	L190
L191:
	cmpl	$255, -12(%ebp)
	jg	L192
	movl	-12(%ebp), %edx
	leal	__register-"L00000000005$pb"(%ebx), %eax
	movl	(%eax,%edx,4), %eax
	incl	-12(%ebp)
	movl	%eax, 4(%esp)
	leal	LC28-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L190:
	decl	-16(%ebp)
	cmpl	$-1, -16(%ebp)
	jne	L191
L192:
	movl	$10, (%esp)
	call	_putchar
	addl	$36, %esp
	popl	%ebx
	leave
	ret
_msw2i:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	movzbl	8(%ebp), %eax
	movzbl	%al, %eax
	movl	%eax, %edx
	sall	$16, %edx
	movzbl	9(%ebp), %eax
	movzbl	%al, %eax
	sall	$8, %eax
	orl	%eax, %edx
	movzbl	10(%ebp), %eax
	movzbl	%al, %eax
	orl	%edx, %eax
	leave
	ret
	.cstring
LC59:
	.ascii "[%p]\0"
LC60:
	.ascii "%2.2x:\0"
	.text
_print_register_row:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$36, %esp
	call	L206
"L00000000006$pb":
L206:
	popl	%ebx
	movl	$8, -12(%ebp)
	leal	L_flag$non_lazy_ptr-"L00000000006$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	4(%eax), %eax
	testb	%al, %al
	je	L199
	leal	__register-"L00000000006$pb"(%ebx), %eax
	movl	%eax, 4(%esp)
	leal	LC59-"L00000000006$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L199:
	movl	8(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC60-"L00000000006$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	jmp	L201
L202:
	movl	8(%ebp), %edx
	leal	__register-"L00000000006$pb"(%ebx), %eax
	movl	(%eax,%edx,4), %eax
	incl	8(%ebp)
	movl	%eax, 4(%esp)
	leal	LC28-"L00000000006$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L201:
	decl	-12(%ebp)
	cmpl	$-1, -12(%ebp)
	je	L205
	cmpl	$255, 8(%ebp)
	jle	L202
L205:
	addl	$36, %esp
	popl	%ebx
	leave
	ret
.lcomm _breakpoint,4,2
.lcomm _indication,4,2
.lcomm _start_time,8,3
.lcomm _u,8,3
.lcomm _time1,8,2
.comm _trace,4,2
.comm _trace1,4,2
.comm _trace2,4,2
.comm _trace3,4,2
.comm _memory,2097152,5
.comm _b0_name,4,2
.comm _base,768,5
.comm _devices,512,5
	.section __IMPORT,__pointers,non_lazy_symbol_pointers
L___stdoutp$non_lazy_ptr:
	.indirect_symbol ___stdoutp
	.long	0
L_memory$non_lazy_ptr:
	.indirect_symbol _memory
	.long	0
L_trace$non_lazy_ptr:
	.indirect_symbol _trace
	.long	0
L_devices$non_lazy_ptr:
	.indirect_symbol _devices
	.long	0
L_arguments$non_lazy_ptr:
	.indirect_symbol _arguments
	.long	0
L___stack_chk_guard$non_lazy_ptr:
	.indirect_symbol ___stack_chk_guard
	.long	0
L_trace1$non_lazy_ptr:
	.indirect_symbol _trace1
	.long	0
L_flag$non_lazy_ptr:
	.indirect_symbol _flag
	.long	0
L_trace2$non_lazy_ptr:
	.indirect_symbol _trace2
	.long	0
L_base$non_lazy_ptr:
	.indirect_symbol _base
	.long	0
L_trace3$non_lazy_ptr:
	.indirect_symbol _trace3
	.long	0
L_argument$non_lazy_ptr:
	.indirect_symbol _argument
	.long	0
L___stdinp$non_lazy_ptr:
	.indirect_symbol ___stdinp
	.long	0
	.subsections_via_symbols
