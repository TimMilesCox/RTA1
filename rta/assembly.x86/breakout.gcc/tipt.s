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
	.ascii "async thread ID %p\12\0"
LC9:
	.ascii "g[break:point] to run\0"
LC10:
	.ascii "s to enter single step\0"
LC11:
	.ascii "key %s\12\12\0"
LC12:
	.ascii "stdin terminated\0"
	.text
.globl _main
_main:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	pushl	%ebx
	subl	$208, %esp
	call	L32
"L00000000001$pb":
L32:
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
	leal	_iselect-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %esi
	orl	$2, %esi
	leal	-52(%ebp), %eax
	movl	%eax, (%esp)
	call	_pthread_attr_init
	movl	%eax, %edx
	leal	L__register$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%esi,4)
	leal	_iselect-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %esi
	orl	$2, %esi
	movl	$0, 12(%esp)
	leal	_emulate-"L00000000001$pb"(%ebx), %eax
	movl	%eax, 8(%esp)
	leal	-52(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	-136(%ebp), %eax
	movl	%eax, (%esp)
	call	_pthread_create
	movl	%eax, %edx
	leal	L__register$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%esi,4)
	movl	-136(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC8-"L00000000001$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	leal	L_flag$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	18(%eax), %eax
	testb	%al, %al
	je	L20
	leal	LC9-"L00000000001$pb"(%ebx), %eax
	movl	%eax, -176(%ebp)
	jmp	L22
L20:
	leal	LC10-"L00000000001$pb"(%ebx), %edx
	movl	%edx, -176(%ebp)
L22:
	movl	-176(%ebp), %ecx
	movl	%ecx, 4(%esp)
	leal	LC11-"L00000000001$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L23:
	leal	_iselect-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %esi
	orl	$2, %esi
	movl	$1000, 8(%esp)
	movl	$1, 4(%esp)
	leal	_attention-"L00000000001$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_poll$UNIX2003
	movl	%eax, %edx
	leal	L__register$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%esi,4)
	leal	_iselect-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$2, %edx
	leal	L__register$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	testl	%eax, %eax
	je	L24
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
	jne	L26
	leal	LC12-"L00000000001$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	jmp	L28
L26:
	movl	-132(%ebp), %eax
	movzbl	(%eax), %eax
	cmpb	$46, %al
	je	L28
	movl	-132(%ebp), %eax
	movl	%eax, (%esp)
	call	_action
L24:
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
	jmp	L23
L28:
	movl	$0, -180(%ebp)
L6:
	movl	-180(%ebp), %eax
	leal	L___stack_chk_guard$non_lazy_ptr-"L00000000001$pb"(%ebx), %edx
	movl	(%edx), %edx
	movl	-12(%ebp), %ecx
	xorl	(%edx), %ecx
	je	L31
	call	___stack_chk_fail
L31:
	addl	$208, %esp
	popl	%ebx
	popl	%esi
	leave
	ret
	.cstring
LC13:
	.ascii "emulation start\0"
LC14:
	.ascii "[%x %p %x %p]\12\0"
	.align 2
LC15:
	.ascii "[0:%8.8x 1:%8.8x 2:%8.8x 3:%8.8x]\12\0"
	.text
_emulate:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$60, %esp
	call	L53
"L00000000002$pb":
L53:
	popl	%ebx
	leal	LC13-"L00000000002$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
L34:
	leal	L_flag$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	18(%eax), %eax
	testb	%al, %al
	je	L35
	leal	_indication-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$2, %edx
	leal	_indication-"L00000000002$pb"(%ebx), %eax
	movl	%edx, (%eax)
L35:
	leal	L_flag$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	4(%eax), %eax
	testb	%al, %al
	je	L37
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
	leal	LC14-"L00000000002$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L37:
	L39: # LASM$engage
	# top of block
	.file "tipt.c"
	.line 271
	pushl %ebp
	.file "tipt.c"
	.line 276
	pushl %edi
	.file "tipt.c"
	.line 277
	pushl %edx
	.file "tipt.c"
	.line 278
	pushl %ecx
	.file "tipt.c"
	.line 279
	pushl %ebx
	.file "tipt.c"
	.line 280
	pushl %eax
	.file "tipt.c"
	.line 285
	movl _register_set, %ebp
	.file "tipt.c"
	.line 286
	movl _apc, %edx
	.file "tipt.c"
	.line 288
	movl 0(%ebp), %eax
	.file "tipt.c"
	.line 289
	movl %eax, _trace
	.file "tipt.c"
	.line 290
	movl 4(%ebp), %eax
	.file "tipt.c"
	.line 291
	movl %eax, _trace1
	.file "tipt.c"
	.line 292
	movl 8(%ebp), %eax
	.file "tipt.c"
	.line 293
	movl %eax, _trace2
	.file "tipt.c"
	.line 294
	movl 12(%ebp), %eax
	.file "tipt.c"
	.line 295
	movl %eax, _trace3
	L40: # LASM$next
	.file "tipt.c"
	.line 297
	movl 0(%edx), %eax
	.file "tipt.c"
	.line 298
	addl $4, %edx
	.file "tipt.c"
	.line 299
	bswapl %eax
	.file "tipt.c"
	.line 300
	call _execute
	.file "tipt.c"
	.line 301
	testl $7, _indication
	.file "tipt.c"
	.line 302
	jz L40
	.file "tipt.c"
	.line 304
	movl %ebp, _register_set
	.file "tipt.c"
	.line 305
	movl %edx, _apc
	.file "tipt.c"
	.line 307
	popl %eax
	.file "tipt.c"
	.line 308
	popl %ebx
	.file "tipt.c"
	.line 309
	popl %ecx
	.file "tipt.c"
	.line 310
	popl %edx
	.file "tipt.c"
	.line 311
	popl %edi
	.file "tipt.c"
	.line 315
	popl %ebp
	leal	L_flag$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	4(%eax), %eax
	testb	%al, %al
	je	L41
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
	leal	LC15-"L00000000002$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L41:
	leal	_indication-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	andl	$1, %eax
	testb	%al, %al
	je	L43
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
	leal	L__register$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, 604(%eax)
	leal	_u-"L00000000002$pb"(%ebx), %eax
	movl	4(%eax), %edx
	movl	(%eax), %eax
	shrdl	$24, %edx, %eax
	sarl	$24, %edx
	movl	%eax, %edx
	andl	$16777215, %edx
	leal	L__register$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
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
	je	L34
L43:
	leal	_indication-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	andl	$4, %eax
	testl	%eax, %eax
	je	L46
	leal	_apc-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %edx
	leal	_breakpoint-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	cmpl	%eax, %edx
	jne	L46
	leal	L_flag$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movb	$1, 18(%eax)
	leal	_indication-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$2, %edx
	leal	_indication-"L00000000002$pb"(%ebx), %eax
	movl	%edx, (%eax)
L46:
	leal	_indication-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	andl	$2, %eax
	testl	%eax, %eax
	je	L34
	call	_statement
	movl	$62, (%esp)
	call	_putchar
	leal	L___stdoutp$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	_fflush
	jmp	L50
L51:
	movl	$10000, (%esp)
	call	_usleep
L50:
	leal	_indication-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	andl	$2, %eax
	testl	%eax, %eax
	jne	L51
	jmp	L34
	.cstring
LC16:
	.ascii "key s for single step\0"
LC17:
	.ascii "%x:%x\0"
LC18:
	.ascii "%x\0"
LC19:
	.ascii "%d\0"
LC20:
	.ascii "%6.6x :\0"
LC21:
	.ascii " %2.2x%2.2x%2.2x\0"
LC22:
	.ascii "%6.6x:%6.6x :\0"
LC23:
	.ascii " %6.6x\0"
LC24:
	.ascii "\12%2.2x:\0"
	.align 2
LC25:
	.ascii "\12"
	.ascii "7c                             \0"
LC26:
	.ascii "%lld\0"
LC27:
	.ascii "%s\0"
	.align 2
LC28:
	.ascii "LF\11execute one instruction and display new state\0"
	.align 2
LC29:
	.ascii "+\11display more internal stack registers\0"
	.align 2
LC30:
	.ascii "0n\11display registers starting at hex address\0"
	.align 2
LC31:
	.ascii "r n\11display registers starting at decimal address\0"
	.align 2
LC32:
	.ascii "b\11display relocation / configuration ports\12\0"
	.align 2
LC33:
	.ascii "m [[page:]offset]\11display system memory\0"
	.align 2
LC34:
	.ascii "\11default page[s] in current address space\12\0"
	.align 2
LC35:
	.ascii "d [[device:]offset]\11display peripheral memory array[s]\0"
	.align 2
LC36:
	.ascii "\11default device[s] in current address space\12\0"
	.align 2
LC37:
	.ascii "g [[page:]breakpoint]\11run [to breakpoint]\0"
	.align 2
LC38:
	.ascii "\11default page is current instruction section\0"
LC39:
	.ascii "\11to remove breakpoint g0:0\12\0"
LC40:
	.ascii ".\11exit emulator\0"
	.text
_action:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$436, %esp
	call	L131
"L00000000003$pb":
L131:
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
	jne	L55
	cmpl	$115, -408(%ebp)
	je	L55
	leal	LC16-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	jmp	L129
L55:
	movl	-408(%ebp), %eax
	subl	$43, %eax
	movl	%eax, -416(%ebp)
	cmpl	$79, -416(%ebp)
	ja	L59
	movl	-416(%ebp), %edx
	sall	$2, %edx
	leal	L71-"L00000000003$pb"(%ebx), %eax
	movl	(%edx,%eax), %eax
	addl	%ebx, %eax
	jmp	*%eax
	.align 2,0x90
L71:
	.long	L60-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L61-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L62-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L63-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L64-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L65-"L00000000003$pb"
	.long	L66-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L67-"L00000000003$pb"
	.long	L68-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L62-"L00000000003$pb"
	.long	L69-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
L69:
	leal	_indication-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$2, %edx
	leal	_indication-"L00000000003$pb"(%ebx), %eax
	movl	%edx, (%eax)
	leal	L_flag$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movb	$1, 18(%eax)
	jmp	L129
L65:
	movl	-412(%ebp), %edx
	incl	%edx
	leal	-400(%ebp), %eax
	movl	%eax, 12(%esp)
	leal	-388(%ebp), %eax
	movl	%eax, 8(%esp)
	leal	LC17-"L00000000003$pb"(%ebx), %eax
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	_sscanf
	movl	%eax, -384(%ebp)
	cmpl	$1, -384(%ebp)
	jne	L72
	leal	_b0p-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	movl	-388(%ebp), %eax
	sall	$2, %eax
	addl	%eax, %edx
	leal	_breakpoint-"L00000000003$pb"(%ebx), %eax
	movl	%edx, (%eax)
L72:
	cmpl	$2, -384(%ebp)
	jne	L74
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
L74:
	leal	L_flag$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movb	$0, 18(%eax)
	leal	_indication-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	andl	$-3, %edx
	leal	_indication-"L00000000003$pb"(%ebx), %eax
	movl	%edx, (%eax)
	movl	$58, (%esp)
	call	_putchar
	leal	L___stdoutp$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	_fflush
	jmp	L129
L60:
	leal	_iselect-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$15, %edx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, -388(%ebp)
	movl	-388(%ebp), %eax
	addl	$23, %eax
	movl	%eax, -388(%ebp)
L62:
	cmpl	$43, -408(%ebp)
	je	L76
	leal	_iselect-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	orl	$24, %eax
	movl	%eax, -388(%ebp)
L76:
	cmpl	$48, -408(%ebp)
	jne	L78
	leal	-388(%ebp), %eax
	movl	%eax, 8(%esp)
	leal	LC18-"L00000000003$pb"(%ebx), %eax
	movl	%eax, 4(%esp)
	movl	-412(%ebp), %eax
	movl	%eax, (%esp)
	call	_sscanf
	jmp	L82
L78:
	cmpl	$114, -408(%ebp)
	jne	L82
	movl	-412(%ebp), %eax
	incl	%eax
	leal	-388(%ebp), %edx
	movl	%edx, 8(%esp)
	leal	LC19-"L00000000003$pb"(%ebx), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	_sscanf
	jmp	L82
L83:
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
	je	L129
L82:
	movl	-388(%ebp), %eax
	cmpl	$255, %eax
	jle	L83
	jmp	L129
L68:
	movl	-412(%ebp), %edx
	incl	%edx
	leal	-400(%ebp), %eax
	movl	%eax, 12(%esp)
	leal	-396(%ebp), %eax
	movl	%eax, 8(%esp)
	leal	LC17-"L00000000003$pb"(%ebx), %eax
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	_sscanf
	movl	%eax, -384(%ebp)
	cmpl	$1, -384(%ebp)
	jg	L85
	movl	-396(%ebp), %eax
	movl	%eax, -400(%ebp)
L87:
	movl	-400(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -388(%ebp)
	movl	-400(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC20-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	jmp	L88
L89:
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
	leal	LC21-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L88:
	movl	-400(%ebp), %edx
	movl	-388(%ebp), %eax
	cmpl	%eax, %edx
	jl	L89
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
	je	L129
	jmp	L87
L85:
	movl	-396(%ebp), %eax
	movl	%eax, %edx
	sall	$12, %edx
	movl	-400(%ebp), %eax
	leal	(%edx,%eax), %eax
	movl	%eax, -404(%ebp)
L94:
	movl	-404(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -388(%ebp)
	movl	-400(%ebp), %eax
	movl	-396(%ebp), %edx
	movl	%eax, 8(%esp)
	movl	%edx, 4(%esp)
	leal	LC22-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	movl	-400(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -400(%ebp)
	jmp	L95
L96:
	movl	-404(%ebp), %eax
	cmpl	$524287, %eax
	jg	L97
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
	leal	LC21-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L95:
	movl	-404(%ebp), %edx
	movl	-388(%ebp), %eax
	cmpl	%eax, %edx
	jl	L96
L97:
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
	je	L129
	jmp	L94
L64:
	movl	$0, -404(%ebp)
	movl	$0, -400(%ebp)
	movl	$0, -392(%ebp)
	movl	-412(%ebp), %edx
	incl	%edx
	leal	-400(%ebp), %eax
	movl	%eax, 12(%esp)
	leal	-404(%ebp), %eax
	movl	%eax, 8(%esp)
	leal	LC17-"L00000000003$pb"(%ebx), %eax
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	_sscanf
	movl	%eax, -384(%ebp)
	cmpl	$1, -384(%ebp)
	jg	L100
	movl	-404(%ebp), %eax
	movl	%eax, -400(%ebp)
	movl	$0, -404(%ebp)
	movl	-400(%ebp), %eax
	andl	$16515072, %eax
	movl	%eax, -392(%ebp)
	cmpl	$0, -392(%ebp)
	je	L102
	sarl	$18, -392(%ebp)
	movl	-400(%ebp), %eax
	andl	$262143, %eax
	movl	%eax, -400(%ebp)
	jmp	L104
L102:
	movl	-400(%ebp), %eax
	andl	$258048, %eax
	movl	%eax, -392(%ebp)
	sarl	$12, -392(%ebp)
	movl	-400(%ebp), %eax
	andl	$4095, %eax
	movl	%eax, -400(%ebp)
L104:
	cmpl	$0, -392(%ebp)
	je	L105
	cmpl	$7, -392(%ebp)
	jg	L105
	movl	-392(%ebp), %ecx
	movl	$32768, %eax
	sarl	%cl, %eax
	movl	%eax, %edx
	leal	_psr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	andl	%edx, %eax
	testl	%eax, %eax
	je	L105
	addl	$64, -392(%ebp)
L105:
	movl	-392(%ebp), %edx
	leal	L_base$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, -396(%ebp)
	movl	-396(%ebp), %eax
	andl	$4194304, %eax
	testl	%eax, %eax
	je	L111
	movl	-396(%ebp), %eax
	andl	$63, %eax
	movl	%eax, -404(%ebp)
	jmp	L111
L100:
	movl	-404(%ebp), %eax
	orl	$4194304, %eax
	movl	%eax, -396(%ebp)
L111:
	movl	-400(%ebp), %eax
	movl	-396(%ebp), %edx
	movl	%eax, 8(%esp)
	movl	%edx, 4(%esp)
	leal	LC22-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	movl	-400(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -388(%ebp)
	jmp	L112
L113:
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
	leal	LC23-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L112:
	movl	-400(%ebp), %edx
	movl	-388(%ebp), %eax
	cmpl	%eax, %edx
	jl	L113
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
	je	L129
	jmp	L111
L63:
	movl	$0, -384(%ebp)
	jmp	L117
L118:
	movl	-384(%ebp), %eax
	andl	$7, %eax
	testl	%eax, %eax
	jne	L119
	movl	-384(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC24-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L119:
	movl	-384(%ebp), %edx
	leal	L_base$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, 4(%esp)
	leal	LC23-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	incl	-384(%ebp)
L117:
	cmpl	$71, -384(%ebp)
	jle	L118
	leal	LC25-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	movl	$124, -384(%ebp)
	jmp	L122
L123:
	movl	-384(%ebp), %eax
	andl	$7, %eax
	testl	%eax, %eax
	jne	L124
	movl	-384(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC24-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L124:
	movl	-384(%ebp), %edx
	leal	L_base$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, 4(%esp)
	leal	LC23-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	incl	-384(%ebp)
L122:
	cmpl	$191, -384(%ebp)
	jle	L123
	movl	$10, (%esp)
	call	_putchar
	jmp	L129
L70:
	movl	-412(%ebp), %eax
	incl	%eax
	leal	_start_time-"L00000000003$pb"(%ebx), %edx
	movl	%edx, 8(%esp)
	leal	LC26-"L00000000003$pb"(%ebx), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	_sscanf
	jmp	L129
L67:
	movl	-412(%ebp), %edx
	incl	%edx
	leal	-372(%ebp), %eax
	movl	%eax, 8(%esp)
	leal	LC27-"L00000000003$pb"(%ebx), %eax
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	_sscanf
	movl	%eax, -384(%ebp)
	cmpl	$1, -384(%ebp)
	jne	L129
	leal	-372(%ebp), %eax
	movl	%eax, (%esp)
	call	_load_fs
	jmp	L129
L66:
	leal	LC28-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	leal	LC29-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	leal	LC30-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	leal	LC31-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	leal	LC32-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
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
	jmp	L129
L61:
	leal	_indication-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	andl	$-3, %edx
	leal	_indication-"L00000000003$pb"(%ebx), %eax
	movl	%edx, (%eax)
	jmp	L129
L59:
	leal	_indication-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	andl	$-3, %edx
	leal	_indication-"L00000000003$pb"(%ebx), %eax
	movl	%edx, (%eax)
L129:
	leal	L___stack_chk_guard$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	-12(%ebp), %edx
	xorl	(%eax), %edx
	je	L130
	call	___stack_chk_fail
L130:
	addl	$436, %esp
	popl	%ebx
	leave
	ret
	.cstring
	.align 2
LC41:
	.ascii "file system image %s error %d\12\0"
LC42:
	.ascii "loading file system image %s\12\0"
	.align 2
LC43:
	.ascii "volume header read unsuccessful\0"
LC44:
	.ascii "[%2.2x%2.2x%2.2x]\12\0"
LC45:
	.ascii "\12%4.4x: \0"
LC46:
	.ascii "%2.2x\0"
LC47:
	.ascii "%.*s %d storage banks\12\0"
	.align 2
LC48:
	.ascii "%d unable to cache file system\12\0"
LC49:
	.ascii "%d granules loaded\12\0"
	.text
_load_fs:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$3124, %esp
	call	L162
"L00000000004$pb":
L162:
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
	jns	L133
	call	___error
	movl	(%eax), %eax
	movl	%eax, 8(%esp)
	movl	8(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC41-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	jmp	L161
L133:
	movl	8(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC42-"L00000000004$pb"(%ebx), %eax
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
	je	L136
	leal	LC43-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	jmp	L161
L136:
	leal	L_flag$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	21(%eax), %eax
	testb	%al, %al
	je	L138
	movzbl	-3053(%ebp), %eax
	movzbl	%al, %edx
	movzbl	-3054(%ebp), %eax
	movzbl	%al, %ecx
	movzbl	-3055(%ebp), %eax
	movzbl	%al, %eax
	movl	%edx, 12(%esp)
	movl	%ecx, 8(%esp)
	movl	%eax, 4(%esp)
	leal	LC44-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L138:
	movzbl	-3055(%ebp), %eax
	cmpb	$86, %al
	je	L140
	leal	L_flag$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	21(%eax), %eax
	testb	%al, %al
	je	L161
	movl	$0, -20(%ebp)
	leal	-3100(%ebp), %eax
	movl	%eax, -24(%ebp)
	jmp	L144
L145:
	movl	-20(%ebp), %eax
	andl	$15, %eax
	testl	%eax, %eax
	jne	L146
	movl	-20(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC45-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L146:
	movl	-24(%ebp), %eax
	movzbl	(%eax), %eax
	movsbl	%al,%eax
	incl	-24(%ebp)
	movl	%eax, 4(%esp)
	leal	LC46-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	incl	-20(%ebp)
L144:
	decl	-12(%ebp)
	cmpl	$-1, -12(%ebp)
	jne	L145
	movl	$10, (%esp)
	call	_putchar
	jmp	L161
L140:
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
	leal	LC47-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	cmpl	$0, -16(%ebp)
	je	L161
	cmpl	$0, -24(%ebp)
	je	L150
	movl	-24(%ebp), %eax
	movl	%eax, (%esp)
	call	_free
L150:
	movl	-16(%ebp), %eax
	movl	$786432, 4(%esp)
	movl	%eax, (%esp)
	call	_calloc
	movl	%eax, -24(%ebp)
	cmpl	$0, -24(%ebp)
	jne	L152
	call	___error
	movl	(%eax), %eax
	movl	%eax, 4(%esp)
	leal	LC48-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	jmp	L161
L152:
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
	je	L154
	movl	$-1, %eax
	movl	%eax, 12(%esp)
	movl	$3072, 8(%esp)
	leal	-3100(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	-24(%ebp), %eax
	movl	%eax, (%esp)
	call	___memcpy_chk
	jmp	L156
L154:
	movl	$3072, 8(%esp)
	leal	-3100(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	-24(%ebp), %eax
	movl	%eax, (%esp)
	call	___inline_memcpy_chk
L156:
	addl	$3072, -24(%ebp)
	movl	$16, -20(%ebp)
L157:
	movl	$192, 8(%esp)
	movl	-24(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	-28(%ebp), %eax
	movl	%eax, (%esp)
	call	_read
	movl	%eax, -12(%ebp)
	cmpl	$0, -12(%ebp)
	js	L158
	cmpl	$0, -12(%ebp)
	je	L158
	incl	-20(%ebp)
	addl	$192, -24(%ebp)
	jmp	L157
L158:
	movl	-28(%ebp), %eax
	movl	%eax, (%esp)
	call	_close
	movl	-20(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC49-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L161:
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
LC50:
	.ascii "%6.6x %8.8x\12\0"
LC51:
	.ascii "[%6.6x]->\0"
LC52:
	.ascii "\12   \0"
	.text
_statement:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$36, %esp
	call	L182
"L00000000005$pb":
L182:
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
	leal	LC50-"L00000000005$pb"(%ebx), %eax
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
	jmp	L166
L167:
	movl	-12(%ebp), %eax
	movl	%eax, (%esp)
	call	_print_register_row
	movl	$10, (%esp)
	call	_putchar
	addl	$8, -12(%ebp)
L166:
	leal	_iselect-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	orl	$24, %eax
	cmpl	-12(%ebp), %eax
	jg	L167
	leal	_iselect-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$15, %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC51-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	movl	$7, -16(%ebp)
	jmp	L169
L170:
	cmpl	$255, -12(%ebp)
	jg	L171
	movl	-12(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	incl	-12(%ebp)
	movl	%eax, 4(%esp)
	leal	LC23-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L169:
	decl	-16(%ebp)
	cmpl	$-1, -16(%ebp)
	jne	L170
L171:
	leal	LC52-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	movl	$8, -16(%ebp)
	jmp	L173
L174:
	cmpl	$255, -12(%ebp)
	jg	L175
	movl	-12(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	incl	-12(%ebp)
	movl	%eax, 4(%esp)
	leal	LC23-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L173:
	decl	-16(%ebp)
	cmpl	$-1, -16(%ebp)
	jne	L174
L175:
	leal	LC52-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	movl	$8, -16(%ebp)
	jmp	L177
L178:
	cmpl	$255, -12(%ebp)
	jg	L179
	movl	-12(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	incl	-12(%ebp)
	movl	%eax, 4(%esp)
	leal	LC23-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L177:
	decl	-16(%ebp)
	cmpl	$-1, -16(%ebp)
	jne	L178
L179:
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
LC53:
	.ascii "%2.2x:\0"
	.text
_print_register_row:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$36, %esp
	call	L191
"L00000000006$pb":
L191:
	popl	%ebx
	movl	$8, -12(%ebp)
	movl	8(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC53-"L00000000006$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	jmp	L186
L187:
	movl	8(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000006$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	incl	8(%ebp)
	movl	%eax, 4(%esp)
	leal	LC23-"L00000000006$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L186:
	decl	-12(%ebp)
	cmpl	$-1, -12(%ebp)
	je	L190
	cmpl	$255, 8(%ebp)
	jle	L187
L190:
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
.comm _b0_name,4,2
.comm __register,1120,5
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
L__register$non_lazy_ptr:
	.indirect_symbol __register
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
