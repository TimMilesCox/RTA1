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
.globl _base
	.align 5
_base:
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
	.long	1
	.long	0
	.long	8388608
	.long	14680064
	.long	8912897
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
.globl _devices
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
.globl _pdevice
	.align 2
_pdevice:
	.long	_devices
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
	movl	%eax, %edx
	sarl	$12, %edx
	leal	_base-"L00000000001$pb"(%ebx), %eax
	movl	%edx, 496(%eax)
	leal	_base-"L00000000001$pb"(%ebx), %eax
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
	leal	_start_time-"L00000000001$pb"(%ebx), %eax
	movl	4(%eax), %edx
	movl	(%eax), %eax
	leal	_delta_base-"L00000000001$pb"(%ebx), %ecx
	movl	%eax, (%ecx)
	movl	%edx, 4(%ecx)
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
	movl	$3, 8(%esp)
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
	leal	L_indication$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$1, %edx
	leal	L_indication$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
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
	.ascii "[%x %p %x %p %x]\12\0"
LC18:
	.ascii "[%x]\0"
	.text
_emulate:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$44, %esp
	call	L62
"L00000000002$pb":
L62:
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
	leal	L_indication$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$2, %edx
	leal	L_indication$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax)
L42:
	leal	L_flag$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	4(%eax), %eax
	testb	%al, %al
	je	L44
	leal	_apc-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	leal	L_memory$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, %ecx
	subl	%eax, %ecx
	movl	%ecx, %eax
	sarl	$2, %eax
	movl	%eax, %edx
	leal	_apc-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %ecx
	leal	_psr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %esi
	leal	_register_set-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %edi
	leal	L_indication$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%edx, 20(%esp)
	movl	%ecx, 16(%esp)
	movl	%esi, 12(%esp)
	movl	%edi, 8(%esp)
	movl	%eax, 4(%esp)
	leal	LC17-"L00000000002$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L44:
	L46: # LASM$engage
	# top of block
	.file "tipt.c"
	.line 341
	pushl %ebp
	.file "tipt.c"
	.line 342
	movl %esi, %ebp
	.file "tipt.c"
	.line 343
	pushl %ebp
	.file "tipt.c"
	.line 348
	pushl %edi
	.file "tipt.c"
	.line 349
	pushl %edx
	.file "tipt.c"
	.line 350
	pushl %ecx
	.file "tipt.c"
	.line 351
	pushl %ebx
	.file "tipt.c"
	.line 352
	pushl %eax
	.file "tipt.c"
	.line 357
	movl _register_set, %ebp
	.file "tipt.c"
	.line 358
	movl _apc, %edx
	L47: # LASM$next
	.file "tipt.c"
	.line 359
	movl 0(%edx), %eax
	.file "tipt.c"
	.line 360
	addl $4, %edx
	.file "tipt.c"
	.line 361
	bswapl %eax
	.file "tipt.c"
	.line 362
	call _execute
	.file "tipt.c"
	.line 365
	incl _metric
	.file "tipt.c"
	.line 368
	testl $15, _indication
	.file "tipt.c"
	.line 369
	jz L47
	.file "tipt.c"
	.line 370
	movl %ebp, _register_set
	.file "tipt.c"
	.line 371
	movl %edx, _apc
	.file "tipt.c"
	.line 373
	popl %eax
	.file "tipt.c"
	.line 374
	popl %ebx
	.file "tipt.c"
	.line 375
	popl %ecx
	.file "tipt.c"
	.line 376
	popl %edx
	.file "tipt.c"
	.line 377
	popl %edi
	.file "tipt.c"
	.line 380
	popl %ebp
	.file "tipt.c"
	.line 381
	movl %ebp, %esi
	.file "tipt.c"
	.line 382
	popl %ebp
	leal	L_flag$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	8(%eax), %eax
	testb	%al, %al
	je	L48
	leal	L_metric$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, 4(%esp)
	leal	LC18-"L00000000002$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	leal	L___stdoutp$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	_fflush
L48:
	leal	L_indication$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$8, %eax
	testl	%eax, %eax
	je	L50
	call	_accumulate_metric
	leal	L_indication$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	andl	$-9, %edx
	leal	L_indication$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax)
	leal	_base-"L00000000002$pb"(%ebx), %eax
	movl	412(%eax), %eax
	movl	%eax, (%esp)
	call	_usleep
L50:
	leal	L_indication$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$1, %eax
	testb	%al, %al
	je	L52
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
	leal	L_indication$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	andl	$-2, %edx
	leal	L_indication$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax)
	leal	L_indication$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	testl	%eax, %eax
	je	L41
L52:
	leal	L_indication$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$4, %eax
	testl	%eax, %eax
	je	L55
	leal	_apc-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %edx
	leal	_breakpoint-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	cmpl	%eax, %edx
	jne	L55
	leal	L_flag$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movb	$1, 18(%eax)
	leal	L_indication$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$2, %edx
	leal	L_indication$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax)
L55:
	leal	L_indication$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
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
	jmp	L59
L60:
	movl	$10000, (%esp)
	call	_usleep
L59:
	leal	L_indication$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$2, %eax
	testl	%eax, %eax
	jne	L60
	jmp	L41
	.cstring
	.align 2
LC19:
	.ascii "instructions %lld millisecs %lld approximate MIPS %d\12\0"
LC20:
	.ascii "key s for single step\0"
LC21:
	.ascii "%x:%x\0"
LC22:
	.ascii "[>%x,%x]\0"
LC23:
	.ascii "[@%p:%p]\0"
LC24:
	.ascii "%x\0"
LC25:
	.ascii "%d\0"
LC26:
	.ascii "%6.6x :\0"
LC27:
	.ascii " %2.2x%2.2x%2.2x\0"
LC28:
	.ascii "%6.6x:%6.6x :\0"
LC29:
	.ascii " %6.6x\0"
LC30:
	.ascii "\12%2.2x:\0"
LC31:
	.ascii "\12\12"
	.ascii "60:\0"
	.align 2
LC32:
	.ascii "\12\12"
	.ascii "7c                             \0"
LC33:
	.ascii "%lld\0"
LC34:
	.ascii "%s\0"
	.align 2
LC35:
	.ascii "LF\11execute one instruction and display new state\0"
	.align 2
LC36:
	.ascii "+\11display more internal stack registers\0"
	.align 2
LC37:
	.ascii "0n\11display registers starting at hex address\0"
	.align 2
LC38:
	.ascii "r n\11display registers starting at decimal address\0"
	.align 2
LC39:
	.ascii "b\11display relocation / configuration ports\12\0"
	.align 2
LC40:
	.ascii "m [[page:]offset]\11display system memory\0"
	.align 2
LC41:
	.ascii "\11default page[s] in current address space\12\0"
	.align 2
LC42:
	.ascii "d [[device:]offset]\11display peripheral memory array[s]\0"
	.align 2
LC43:
	.ascii "\11default device[s] in current address space\12\0"
	.align 2
LC44:
	.ascii "g [[page:]breakpoint]\11run [to breakpoint]\0"
	.align 2
LC45:
	.ascii "\11default page is current instruction section\0"
LC46:
	.ascii "\11to remove breakpoint g0:0\12\0"
LC47:
	.ascii ".\11exit emulator\0"
	.text
_action:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$492, %esp
	call	L156
"L00000000003$pb":
L156:
	popl	%ebx
	movl	8(%ebp), %eax
	movl	%eax, -444(%ebp)
	leal	L___stack_chk_guard$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %edx
	movl	%edx, -28(%ebp)
	xorl	%edx, %edx
	movl	-444(%ebp), %eax
	movzbl	(%eax), %eax
	movsbl	%al,%eax
	movl	%eax, -424(%ebp)
	movl	$0, -428(%ebp)
	cmpl	$105, -424(%ebp)
	jne	L64
	leal	_total_metric-"L00000000003$pb"(%ebx), %eax
	movl	4(%eax), %edx
	movl	(%eax), %eax
	movl	%eax, -456(%ebp)
	movl	%eax, %ecx
	sarl	$31, %ecx
	movl	%ecx, -452(%ebp)
	leal	_total_delta-"L00000000003$pb"(%ebx), %eax
	movl	4(%eax), %edx
	movl	(%eax), %eax
	imull	$1000, %edx, %ecx
	imull	$0, %eax, %esi
	addl	%esi, %ecx
	movl	$1000, %esi
	mull	%esi
	addl	%edx, %ecx
	movl	%ecx, %edx
	movl	%eax, 8(%esp)
	movl	%edx, 12(%esp)
	movl	-456(%ebp), %eax
	movl	-452(%ebp), %edx
	movl	%eax, (%esp)
	movl	%edx, 4(%esp)
	call	___udivdi3
	movl	%eax, -400(%ebp)
	leal	_total_delta-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %esi
	movl	4(%eax), %edi
	leal	_total_metric-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %edx
	movl	4(%eax), %ecx
	movl	-400(%ebp), %eax
	movl	%eax, 20(%esp)
	movl	%esi, 12(%esp)
	movl	%edi, 16(%esp)
	movl	%edx, 4(%esp)
	movl	%ecx, 8(%esp)
	leal	LC19-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	jmp	L154
L64:
	leal	L_flag$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	18(%eax), %eax
	testb	%al, %al
	jne	L67
	cmpl	$115, -424(%ebp)
	je	L67
	leal	LC20-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	jmp	L154
L67:
	movl	$1, -428(%ebp)
	movl	-424(%ebp), %edx
	subl	$43, %edx
	movl	%edx, -460(%ebp)
	cmpl	$79, -460(%ebp)
	ja	L70
	movl	-460(%ebp), %edx
	sall	$2, %edx
	leal	L82-"L00000000003$pb"(%ebx), %eax
	movl	(%edx,%eax), %eax
	addl	%ebx, %eax
	jmp	*%eax
	.align 2,0x90
L82:
	.long	L71-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L72-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L73-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L74-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L75-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L76-"L00000000003$pb"
	.long	L77-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L78-"L00000000003$pb"
	.long	L79-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L73-"L00000000003$pb"
	.long	L80-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L70-"L00000000003$pb"
	.long	L81-"L00000000003$pb"
L80:
	leal	L_indication$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$2, %edx
	leal	L_indication$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax)
	leal	L_flag$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movb	$1, 18(%eax)
	jmp	L83
L76:
	movl	-444(%ebp), %edx
	incl	%edx
	leal	-416(%ebp), %eax
	movl	%eax, 12(%esp)
	leal	-404(%ebp), %eax
	movl	%eax, 8(%esp)
	leal	LC21-"L00000000003$pb"(%ebx), %eax
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	_sscanf
	movl	%eax, -400(%ebp)
	cmpl	$1, -400(%ebp)
	jne	L84
	leal	_b0p-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	movl	-404(%ebp), %eax
	sall	$2, %eax
	addl	%eax, %edx
	leal	_breakpoint-"L00000000003$pb"(%ebx), %eax
	movl	%edx, (%eax)
L84:
	cmpl	$2, -400(%ebp)
	jne	L86
	movl	-404(%ebp), %eax
	movl	%eax, %edx
	sall	$14, %edx
	leal	L_memory$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	addl	%eax, %edx
	movl	-416(%ebp), %eax
	sall	$2, %eax
	addl	%eax, %edx
	leal	_breakpoint-"L00000000003$pb"(%ebx), %eax
	movl	%edx, (%eax)
L86:
	leal	L_flag$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movb	$0, 18(%eax)
	leal	L_indication$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	andl	$-3, %edx
	leal	L_indication$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax)
	cmpl	$0, -400(%ebp)
	jg	L88
	movl	$0, -400(%ebp)
L88:
	cmpl	$0, -400(%ebp)
	je	L90
	leal	L_indication$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$4, %edx
	leal	L_indication$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax)
L90:
	leal	_breakpoint-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	testl	%eax, %eax
	je	L92
	leal	L_flag$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	21(%eax), %eax
	testb	%al, %al
	je	L94
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
	leal	L_indication$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%edx, 8(%esp)
	movl	%eax, 4(%esp)
	leal	LC22-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L94:
	leal	L_flag$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	4(%eax), %eax
	testb	%al, %al
	je	L92
	leal	_breakpoint-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, 8(%esp)
	leal	L_memory$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, 4(%esp)
	leal	LC23-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L92:
	movl	$0, -428(%ebp)
	movl	$58, (%esp)
	call	_putchar
	leal	L___stdoutp$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	_fflush
	jmp	L83
L71:
	leal	_iselect-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$15, %edx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, -404(%ebp)
	movl	-404(%ebp), %eax
	addl	$23, %eax
	movl	%eax, -404(%ebp)
L73:
	cmpl	$43, -424(%ebp)
	je	L97
	leal	_iselect-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	orl	$24, %eax
	movl	%eax, -404(%ebp)
L97:
	cmpl	$48, -424(%ebp)
	jne	L99
	leal	-404(%ebp), %eax
	movl	%eax, 8(%esp)
	leal	LC24-"L00000000003$pb"(%ebx), %eax
	movl	%eax, 4(%esp)
	movl	-444(%ebp), %eax
	movl	%eax, (%esp)
	call	_sscanf
	jmp	L103
L99:
	cmpl	$114, -424(%ebp)
	jne	L103
	movl	-444(%ebp), %eax
	incl	%eax
	leal	-404(%ebp), %edx
	movl	%edx, 8(%esp)
	leal	LC25-"L00000000003$pb"(%ebx), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	_sscanf
	jmp	L103
L104:
	movl	-404(%ebp), %eax
	movl	%eax, (%esp)
	call	_print_register_row
	movl	-404(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -404(%ebp)
	leal	L___stdinp$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, 8(%esp)
	movl	$48, 4(%esp)
	movl	-444(%ebp), %eax
	movl	%eax, (%esp)
	call	_fgets
	movl	-444(%ebp), %eax
	movzbl	(%eax), %eax
	cmpb	$46, %al
	je	L83
L103:
	movl	-404(%ebp), %eax
	cmpl	$279, %eax
	jle	L104
	jmp	L83
L79:
	movl	-444(%ebp), %edx
	incl	%edx
	leal	-416(%ebp), %eax
	movl	%eax, 12(%esp)
	leal	-412(%ebp), %eax
	movl	%eax, 8(%esp)
	leal	LC21-"L00000000003$pb"(%ebx), %eax
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	_sscanf
	movl	%eax, -400(%ebp)
	cmpl	$1, -400(%ebp)
	jg	L106
	movl	-412(%ebp), %eax
	movl	%eax, -416(%ebp)
L108:
	movl	-416(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -404(%ebp)
	movl	-416(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC26-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	jmp	L109
L110:
	movl	-416(%ebp), %eax
	movl	%eax, %edx
	incl	%eax
	movl	%eax, -416(%ebp)
	movl	%edx, (%esp)
	call	_memory_read
	movl	%eax, -392(%ebp)
	movzbl	-389(%ebp), %eax
	movzbl	%al, %edx
	movzbl	-390(%ebp), %eax
	movzbl	%al, %ecx
	movzbl	-391(%ebp), %eax
	movzbl	%al, %eax
	movl	%edx, 12(%esp)
	movl	%ecx, 8(%esp)
	movl	%eax, 4(%esp)
	leal	LC27-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L109:
	movl	-416(%ebp), %edx
	movl	-404(%ebp), %eax
	cmpl	%eax, %edx
	jl	L110
	leal	L___stdinp$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, 8(%esp)
	movl	$48, 4(%esp)
	movl	-444(%ebp), %eax
	movl	%eax, (%esp)
	call	_fgets
	movl	-444(%ebp), %eax
	movzbl	(%eax), %eax
	cmpb	$46, %al
	je	L83
	jmp	L108
L106:
	movl	-412(%ebp), %eax
	movl	%eax, %edx
	sall	$12, %edx
	movl	-416(%ebp), %eax
	leal	(%edx,%eax), %eax
	movl	%eax, -420(%ebp)
L115:
	movl	-420(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -404(%ebp)
	movl	-416(%ebp), %eax
	movl	-412(%ebp), %edx
	movl	%eax, 8(%esp)
	movl	%edx, 4(%esp)
	leal	LC28-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	movl	-416(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -416(%ebp)
	jmp	L116
L117:
	movl	-420(%ebp), %eax
	cmpl	$524287, %eax
	jg	L118
	movl	-420(%ebp), %edx
	leal	L_memory$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, -392(%ebp)
	leal	1(%edx), %eax
	movl	%eax, -420(%ebp)
	movzbl	-389(%ebp), %eax
	movzbl	%al, %edx
	movzbl	-390(%ebp), %eax
	movzbl	%al, %ecx
	movzbl	-391(%ebp), %eax
	movzbl	%al, %eax
	movl	%edx, 12(%esp)
	movl	%ecx, 8(%esp)
	movl	%eax, 4(%esp)
	leal	LC27-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L116:
	movl	-420(%ebp), %edx
	movl	-404(%ebp), %eax
	cmpl	%eax, %edx
	jl	L117
L118:
	leal	L___stdinp$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, 8(%esp)
	movl	$48, 4(%esp)
	movl	-444(%ebp), %eax
	movl	%eax, (%esp)
	call	_fgets
	movl	-444(%ebp), %eax
	movzbl	(%eax), %eax
	cmpb	$46, %al
	je	L83
	jmp	L115
L75:
	movl	$0, -420(%ebp)
	movl	$0, -416(%ebp)
	movl	$0, -408(%ebp)
	movl	-444(%ebp), %edx
	incl	%edx
	leal	-416(%ebp), %eax
	movl	%eax, 12(%esp)
	leal	-420(%ebp), %eax
	movl	%eax, 8(%esp)
	leal	LC21-"L00000000003$pb"(%ebx), %eax
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	_sscanf
	movl	%eax, -400(%ebp)
	cmpl	$1, -400(%ebp)
	jg	L121
	movl	-420(%ebp), %eax
	movl	%eax, -416(%ebp)
	movl	$0, -420(%ebp)
	movl	-416(%ebp), %eax
	andl	$16515072, %eax
	movl	%eax, -408(%ebp)
	cmpl	$0, -408(%ebp)
	je	L123
	sarl	$18, -408(%ebp)
	movl	-416(%ebp), %eax
	andl	$262143, %eax
	movl	%eax, -416(%ebp)
	jmp	L125
L123:
	movl	-416(%ebp), %eax
	andl	$258048, %eax
	movl	%eax, -408(%ebp)
	sarl	$12, -408(%ebp)
	movl	-416(%ebp), %eax
	andl	$4095, %eax
	movl	%eax, -416(%ebp)
L125:
	cmpl	$0, -408(%ebp)
	je	L126
	cmpl	$7, -408(%ebp)
	jg	L126
	movl	-408(%ebp), %ecx
	movl	$32768, %eax
	sarl	%cl, %eax
	movl	%eax, %edx
	leal	_psr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	andl	%edx, %eax
	testl	%eax, %eax
	je	L126
	addl	$64, -408(%ebp)
L126:
	movl	-408(%ebp), %edx
	leal	_base-"L00000000003$pb"(%ebx), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, -412(%ebp)
	movl	-412(%ebp), %eax
	andl	$4194304, %eax
	testl	%eax, %eax
	je	L132
	movl	-412(%ebp), %eax
	andl	$63, %eax
	movl	%eax, -420(%ebp)
	jmp	L132
L121:
	movl	-420(%ebp), %eax
	orl	$4194304, %eax
	movl	%eax, -412(%ebp)
L132:
	movl	-416(%ebp), %eax
	movl	-412(%ebp), %edx
	movl	%eax, 8(%esp)
	movl	%edx, 4(%esp)
	leal	LC28-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	movl	-416(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -404(%ebp)
	jmp	L133
L134:
	movl	-416(%ebp), %eax
	movl	%eax, %edx
	incl	%eax
	movl	%eax, -416(%ebp)
	movl	-412(%ebp), %eax
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	_device_array_read
	movl	%eax, -400(%ebp)
	movl	-400(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC29-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L133:
	movl	-416(%ebp), %edx
	movl	-404(%ebp), %eax
	cmpl	%eax, %edx
	jl	L134
	leal	L___stdinp$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, 8(%esp)
	movl	$48, 4(%esp)
	movl	-444(%ebp), %eax
	movl	%eax, (%esp)
	call	_fgets
	movl	-444(%ebp), %eax
	movzbl	(%eax), %eax
	cmpb	$46, %al
	je	L83
	jmp	L132
L74:
	movl	$0, -400(%ebp)
	jmp	L138
L139:
	movl	-400(%ebp), %eax
	andl	$7, %eax
	testl	%eax, %eax
	jne	L140
	movl	-400(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC30-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L140:
	movl	-400(%ebp), %edx
	leal	_base-"L00000000003$pb"(%ebx), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, 4(%esp)
	leal	LC29-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	incl	-400(%ebp)
L138:
	cmpl	$71, -400(%ebp)
	jle	L139
	leal	LC31-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	movl	$96, -400(%ebp)
	jmp	L143
L144:
	movl	-400(%ebp), %edx
	leal	_base-"L00000000003$pb"(%ebx), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, 4(%esp)
	leal	LC29-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	incl	-400(%ebp)
L143:
	cmpl	$103, -400(%ebp)
	jle	L144
	leal	LC32-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	movl	$124, -400(%ebp)
	jmp	L146
L147:
	movl	-400(%ebp), %eax
	andl	$7, %eax
	testl	%eax, %eax
	jne	L148
	movl	-400(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC30-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L148:
	movl	-400(%ebp), %edx
	leal	_base-"L00000000003$pb"(%ebx), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, 4(%esp)
	leal	LC29-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	incl	-400(%ebp)
L146:
	cmpl	$191, -400(%ebp)
	jle	L147
	movl	$10, (%esp)
	call	_putchar
	jmp	L83
L81:
	movl	-444(%ebp), %eax
	incl	%eax
	leal	_start_time-"L00000000003$pb"(%ebx), %edx
	movl	%edx, 8(%esp)
	leal	LC33-"L00000000003$pb"(%ebx), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	_sscanf
	jmp	L83
L78:
	movl	-444(%ebp), %edx
	incl	%edx
	leal	-388(%ebp), %eax
	movl	%eax, 8(%esp)
	leal	LC34-"L00000000003$pb"(%ebx), %eax
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	_sscanf
	movl	%eax, -400(%ebp)
	cmpl	$1, -400(%ebp)
	jne	L83
	leal	-388(%ebp), %eax
	movl	%eax, (%esp)
	call	_load_fs
	jmp	L83
L77:
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
	leal	LC46-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	leal	LC47-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	jmp	L83
L72:
	movl	$0, -428(%ebp)
	leal	L_indication$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	andl	$-3, %edx
	leal	L_indication$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax)
	jmp	L83
L70:
	movl	$0, -428(%ebp)
	leal	L_indication$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	andl	$-3, %edx
	leal	L_indication$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax)
L83:
	cmpl	$0, -428(%ebp)
	je	L154
	movl	$62, (%esp)
	call	_putchar
	leal	L___stdoutp$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	_fflush
L154:
	leal	L___stack_chk_guard$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	-28(%ebp), %edx
	xorl	(%eax), %edx
	je	L155
	call	___stack_chk_fail
L155:
	addl	$492, %esp
	popl	%ebx
	popl	%esi
	popl	%edi
	leave
	ret
	.cstring
	.align 2
LC48:
	.ascii "file system image %s error %d\12\0"
LC49:
	.ascii "loading file system image %s\12\0"
	.align 2
LC50:
	.ascii "volume header read unsuccessful\0"
LC51:
	.ascii "[%2.2x%2.2x%2.2x]\12\0"
LC52:
	.ascii "\12%4.4x: \0"
LC53:
	.ascii "%2.2x\0"
LC54:
	.ascii "%.*s %d storage banks\12\0"
	.align 2
LC55:
	.ascii "%d unable to cache file system\12\0"
LC56:
	.ascii "%d granules loaded\12\0"
	.text
_load_fs:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$3124, %esp
	call	L187
"L00000000004$pb":
L187:
	popl	%ebx
	movl	$0, -20(%ebp)
	leal	_devices-"L00000000004$pb"(%ebx), %eax
	movl	12(%eax), %eax
	movl	%eax, -24(%ebp)
	movl	$292, 8(%esp)
	movl	$0, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_open$UNIX2003
	movl	%eax, -28(%ebp)
	cmpl	$0, -28(%ebp)
	jns	L158
	call	___error
	movl	(%eax), %eax
	movl	%eax, 8(%esp)
	movl	8(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC48-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	jmp	L186
L158:
	movl	8(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC49-"L00000000004$pb"(%ebx), %eax
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
	je	L161
	leal	LC50-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	jmp	L186
L161:
	leal	L_flag$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	21(%eax), %eax
	testb	%al, %al
	je	L163
	movzbl	-3053(%ebp), %eax
	movzbl	%al, %edx
	movzbl	-3054(%ebp), %eax
	movzbl	%al, %ecx
	movzbl	-3055(%ebp), %eax
	movzbl	%al, %eax
	movl	%edx, 12(%esp)
	movl	%ecx, 8(%esp)
	movl	%eax, 4(%esp)
	leal	LC51-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L163:
	movzbl	-3055(%ebp), %eax
	cmpb	$86, %al
	je	L165
	leal	L_flag$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	21(%eax), %eax
	testb	%al, %al
	je	L186
	movl	$0, -20(%ebp)
	leal	-3100(%ebp), %eax
	movl	%eax, -24(%ebp)
	jmp	L169
L170:
	movl	-20(%ebp), %eax
	andl	$15, %eax
	testl	%eax, %eax
	jne	L171
	movl	-20(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC52-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L171:
	movl	-24(%ebp), %eax
	movzbl	(%eax), %eax
	movsbl	%al,%eax
	incl	-24(%ebp)
	movl	%eax, 4(%esp)
	leal	LC53-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	incl	-20(%ebp)
L169:
	decl	-12(%ebp)
	cmpl	$-1, -12(%ebp)
	jne	L170
	movl	$10, (%esp)
	call	_putchar
	jmp	L186
L165:
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
	leal	LC54-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	cmpl	$0, -16(%ebp)
	je	L186
	cmpl	$0, -24(%ebp)
	je	L175
	movl	-24(%ebp), %eax
	movl	%eax, (%esp)
	call	_free
L175:
	movl	-16(%ebp), %eax
	movl	$786432, 4(%esp)
	movl	%eax, (%esp)
	call	_calloc
	movl	%eax, -24(%ebp)
	cmpl	$0, -24(%ebp)
	jne	L177
	call	___error
	movl	(%eax), %eax
	movl	%eax, 4(%esp)
	leal	LC55-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	jmp	L186
L177:
	movl	-24(%ebp), %edx
	leal	_devices-"L00000000004$pb"(%ebx), %eax
	movl	%edx, 12(%eax)
	movl	-16(%ebp), %edx
	orl	$14680064, %edx
	leal	_base-"L00000000004$pb"(%ebx), %eax
	movl	%edx, 516(%eax)
	movl	$-1, %eax
	cmpl	$-1, %eax
	je	L179
	movl	$-1, %eax
	movl	%eax, 12(%esp)
	movl	$3072, 8(%esp)
	leal	-3100(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	-24(%ebp), %eax
	movl	%eax, (%esp)
	call	___memcpy_chk
	jmp	L181
L179:
	movl	$3072, 8(%esp)
	leal	-3100(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	-24(%ebp), %eax
	movl	%eax, (%esp)
	call	___inline_memcpy_chk
L181:
	addl	$3072, -24(%ebp)
	movl	$16, -20(%ebp)
L182:
	movl	$192, 8(%esp)
	movl	-24(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	-28(%ebp), %eax
	movl	%eax, (%esp)
	call	_read
	movl	%eax, -12(%ebp)
	cmpl	$0, -12(%ebp)
	js	L183
	cmpl	$0, -12(%ebp)
	je	L183
	incl	-20(%ebp)
	addl	$192, -24(%ebp)
	jmp	L182
L183:
	movl	-28(%ebp), %eax
	movl	%eax, (%esp)
	call	_close
	movl	-20(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC56-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L186:
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
LC57:
	.ascii "[RP %p]\0"
LC58:
	.ascii "%6.6x %8.8x\12\0"
LC59:
	.ascii "[%6.6x]->\0"
LC60:
	.ascii "\12   \0"
	.text
_statement:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$36, %esp
	call	L209
"L00000000005$pb":
L209:
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
	je	L191
	leal	_register_set-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, 4(%esp)
	leal	LC57-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L191:
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
	leal	LC58-"L00000000005$pb"(%ebx), %eax
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
	jmp	L193
L194:
	movl	-12(%ebp), %eax
	movl	%eax, (%esp)
	call	_print_register_row
	movl	$10, (%esp)
	call	_putchar
	addl	$8, -12(%ebp)
L193:
	leal	_iselect-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	orl	$24, %eax
	cmpl	-12(%ebp), %eax
	jg	L194
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
	leal	LC59-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	movl	$7, -16(%ebp)
	jmp	L196
L197:
	cmpl	$255, -12(%ebp)
	jg	L198
	movl	-12(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	incl	-12(%ebp)
	movl	%eax, 4(%esp)
	leal	LC29-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L196:
	decl	-16(%ebp)
	cmpl	$-1, -16(%ebp)
	jne	L197
L198:
	leal	LC60-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	movl	$8, -16(%ebp)
	jmp	L200
L201:
	cmpl	$255, -12(%ebp)
	jg	L202
	movl	-12(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	incl	-12(%ebp)
	movl	%eax, 4(%esp)
	leal	LC29-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L200:
	decl	-16(%ebp)
	cmpl	$-1, -16(%ebp)
	jne	L201
L202:
	leal	LC60-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	movl	$8, -16(%ebp)
	jmp	L204
L205:
	cmpl	$255, -12(%ebp)
	jg	L206
	movl	-12(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	incl	-12(%ebp)
	movl	%eax, 4(%esp)
	leal	LC29-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L204:
	decl	-16(%ebp)
	cmpl	$-1, -16(%ebp)
	jne	L205
L206:
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
LC61:
	.ascii "[%p]\0"
LC62:
	.ascii "%2.2x:\0"
	.text
_print_register_row:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$36, %esp
	call	L220
"L00000000006$pb":
L220:
	popl	%ebx
	movl	$8, -12(%ebp)
	leal	L_flag$non_lazy_ptr-"L00000000006$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	4(%eax), %eax
	testb	%al, %al
	je	L213
	leal	L__register$non_lazy_ptr-"L00000000006$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, 4(%esp)
	leal	LC61-"L00000000006$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L213:
	movl	8(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC62-"L00000000006$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	jmp	L215
L216:
	movl	8(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000006$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	incl	8(%ebp)
	movl	%eax, 4(%esp)
	leal	LC29-"L00000000006$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L215:
	decl	-12(%ebp)
	cmpl	$-1, -12(%ebp)
	je	L219
	cmpl	$279, 8(%ebp)
	jle	L216
L219:
	addl	$36, %esp
	popl	%ebx
	leave
	ret
_memory_read:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	pushl	%esi
	subl	$16, %esp
	# top of block
	.file "tipt.c"
	.line 879
	movl 8(%ebp), %eax
	.file "tipt.c"
	.line 880
	pushl %ecx
	.file "tipt.c"
	.line 881
	movl $0, %ecx
	.file "tipt.c"
	.line 882
	call _bus_read
	.file "tipt.c"
	.line 883
	popl %ecx
	.file "tipt.c"
	.line 884
	bswapl %eax
	.file "tipt.c"
	.line 885
	movl %eax, -12(%ebp)
	movl	-12(%ebp), %eax
	addl	$16, %esp
	popl	%esi
	popl	%edi
	leave
	ret
_device_array_read:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	pushl	%esi
	subl	$16, %esp
	movl	8(%ebp), %eax
	movl	%eax, -16(%ebp)
	movl	12(%ebp), %eax
	movl	%eax, -20(%ebp)
	# top of block
	.file "tipt.c"
	.line 906
	pushl %ecx
	.file "tipt.c"
	.line 907
	movl %esi, %ecx
	.file "tipt.c"
	.line 908
	pushl %ecx
	.file "tipt.c"
	.line 909
	xorl %ecx, %ecx
	.file "tipt.c"
	.line 910
	movl -16(%ebp), %eax
	.file "tipt.c"
	.line 911
	movl -20(%ebp), %esi
	.file "tipt.c"
	.line 912
	call _device_read
	.file "tipt.c"
	.line 913
	popl %ecx
	.file "tipt.c"
	.line 914
	movl %ecx, %esi
	.file "tipt.c"
	.line 915
	popl %ecx
	.file "tipt.c"
	.line 916
	movl %eax, -12(%ebp)
	movl	-12(%ebp), %eax
	addl	$16, %esp
	popl	%esi
	popl	%edi
	leave
	ret
_accumulate_metric:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$12, %esp
	call	L227
"L00000000007$pb":
L227:
	popl	%ebx
	leal	_u-"L00000000007$pb"(%ebx), %eax
	movl	4(%eax), %edx
	movl	(%eax), %eax
	movl	%eax, %ecx
	leal	_delta_base-"L00000000007$pb"(%ebx), %eax
	movl	4(%eax), %edx
	movl	(%eax), %eax
	movl	%ecx, %edx
	subl	%eax, %edx
	leal	L_delta$non_lazy_ptr-"L00000000007$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax)
	leal	_u-"L00000000007$pb"(%ebx), %eax
	movl	4(%eax), %edx
	movl	(%eax), %eax
	leal	_delta_base-"L00000000007$pb"(%ebx), %ecx
	movl	%eax, (%ecx)
	movl	%edx, 4(%ecx)
	leal	L_metric$non_lazy_ptr-"L00000000007$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %esi
	movl	$0, %edi
	leal	_total_metric-"L00000000007$pb"(%ebx), %eax
	movl	4(%eax), %edx
	movl	(%eax), %eax
	addl	%esi, %eax
	adcl	%edi, %edx
	leal	_total_metric-"L00000000007$pb"(%ebx), %ecx
	movl	%eax, (%ecx)
	movl	%edx, 4(%ecx)
	leal	L_metric$non_lazy_ptr-"L00000000007$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	$0, (%eax)
	leal	L_delta$non_lazy_ptr-"L00000000007$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %esi
	movl	$0, %edi
	leal	_total_delta-"L00000000007$pb"(%ebx), %eax
	movl	4(%eax), %edx
	movl	(%eax), %eax
	addl	%esi, %eax
	adcl	%edi, %edx
	leal	_total_delta-"L00000000007$pb"(%ebx), %ecx
	movl	%eax, (%ecx)
	movl	%edx, 4(%ecx)
	addl	$12, %esp
	popl	%ebx
	popl	%esi
	popl	%edi
	leave
	ret
.lcomm _breakpoint,4,2
.lcomm _start_time,8,3
.lcomm _u,8,3
.lcomm _time1,8,2
.lcomm _delta_base,8,3
.lcomm _total_delta,8,3
.lcomm _total_metric,8,3
.comm _indication,4,2
.comm _memory,2097152,5
.comm _b0_name,4,2
.comm __register,1120,5
.comm _delta,4,2
.comm _metric,4,2
	.section __IMPORT,__pointers,non_lazy_symbol_pointers
L___stdoutp$non_lazy_ptr:
	.indirect_symbol ___stdoutp
	.long	0
L_memory$non_lazy_ptr:
	.indirect_symbol _memory
	.long	0
L__register$non_lazy_ptr:
	.indirect_symbol __register
	.long	0
L_arguments$non_lazy_ptr:
	.indirect_symbol _arguments
	.long	0
L___stack_chk_guard$non_lazy_ptr:
	.indirect_symbol ___stack_chk_guard
	.long	0
L_indication$non_lazy_ptr:
	.indirect_symbol _indication
	.long	0
L_flag$non_lazy_ptr:
	.indirect_symbol _flag
	.long	0
L_metric$non_lazy_ptr:
	.indirect_symbol _metric
	.long	0
L_delta$non_lazy_ptr:
	.indirect_symbol _delta
	.long	0
L_argument$non_lazy_ptr:
	.indirect_symbol _argument
	.long	0
L___stdinp$non_lazy_ptr:
	.indirect_symbol ___stdinp
	.long	0
	.subsections_via_symbols
