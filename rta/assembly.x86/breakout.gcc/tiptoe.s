	.cstring
	.align 2
LC0:
	.ascii "please key console input again\0"
LC1:
	.ascii "[%x %p %x %x]\12\0"
	.text
_async:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	pushl	%ebx
	subl	$416, %esp
	call	L15
"L00000000001$pb":
L15:
	popl	%ebx
	leal	L___stack_chk_guard$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %edx
	movl	%edx, -12(%ebp)
	xorl	%edx, %edx
	leal	L___stdinp$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	_funlockfile
	leal	L_flag$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	18(%eax), %eax
	testb	%al, %al
	jne	L2
	movl	$58, (%esp)
	call	_putchar
	leal	L___stdoutp$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	_fflush
L2:
	leal	L_flag$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	18(%eax), %eax
	testb	%al, %al
	je	L4
	movl	$62, (%esp)
	call	_putchar
	leal	L___stdoutp$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	_fflush
L4:
	movb	$0, -384(%ebp)
	leal	L___stdinp$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, 8(%esp)
	movl	$360, 4(%esp)
	leal	-384(%ebp), %eax
	movl	%eax, (%esp)
	call	_fgets
	movl	%eax, -388(%ebp)
	cmpl	$0, -388(%ebp)
	je	L6
	movl	-388(%ebp), %eax
	movl	%eax, (%esp)
	call	_action
	jmp	L8
L6:
	movzbl	-384(%ebp), %eax
	testb	%al, %al
	je	L9
	leal	-384(%ebp), %eax
	movl	%eax, (%esp)
	call	_action
	jmp	L8
L9:
	leal	LC0-"L00000000001$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
L8:
	leal	L_flag$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	21(%eax), %eax
	testb	%al, %al
	je	L2
	leal	_lockstep-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %ecx
	leal	_runout-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %esi
	leal	L_flag$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	18(%eax), %eax
	movsbl	%al,%edx
	movl	%ecx, 16(%esp)
	movl	%esi, 12(%esp)
	movl	-388(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	%edx, 4(%esp)
	leal	LC1-"L00000000001$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	jmp	L2
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
LC2:
	.ascii "%2.2x:\0"
LC3:
	.ascii " %6.6x\0"
	.text
_print_register_row:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$36, %esp
	call	L24
"L00000000002$pb":
L24:
	popl	%ebx
	movl	$8, -12(%ebp)
	movl	8(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC2-"L00000000002$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	jmp	L19
L20:
	movl	8(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	incl	8(%ebp)
	movl	%eax, 4(%esp)
	leal	LC3-"L00000000002$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L19:
	decl	-12(%ebp)
	cmpl	$-1, -12(%ebp)
	je	L23
	cmpl	$255, 8(%ebp)
	jle	L20
L23:
	addl	$36, %esp
	popl	%ebx
	leave
	ret
	.cstring
LC4:
	.ascii "%6.6x %8.8x\12\0"
LC5:
	.ascii "[%6.6x]->\0"
LC6:
	.ascii "\12   \0"
	.text
_statement:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$36, %esp
	call	L42
"L00000000003$pb":
L42:
	popl	%ebx
	leal	L_iselect$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, -12(%ebp)
	leal	L_flag$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	11(%eax), %eax
	movsbl	%al,%edx
	leal	L_apc$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	subl	$4, %eax
	movl	%edx, 8(%esp)
	movl	$1, 4(%esp)
	movl	%eax, (%esp)
	call	_instruction_display
	leal	L_apc$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	leal	L_memory$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, %ecx
	subl	%eax, %ecx
	movl	%ecx, %eax
	sarl	$2, %eax
	movl	%eax, %edx
	leal	L_psr$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%edx, 8(%esp)
	movl	%eax, 4(%esp)
	leal	LC4-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	leal	L_flag$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	11(%eax), %eax
	movsbl	%al,%edx
	leal	L_apc$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%edx, 8(%esp)
	movl	$6, 4(%esp)
	movl	%eax, (%esp)
	call	_instruction_display
	jmp	L26
L27:
	movl	-12(%ebp), %eax
	movl	%eax, (%esp)
	call	_print_register_row
	movl	$10, (%esp)
	call	_putchar
	addl	$8, -12(%ebp)
L26:
	leal	L_iselect$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	orl	$24, %eax
	cmpl	-12(%ebp), %eax
	jg	L27
	leal	L_iselect$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$15, %edx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC5-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	movl	$7, -16(%ebp)
	jmp	L29
L30:
	cmpl	$255, -12(%ebp)
	jg	L31
	movl	-12(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	incl	-12(%ebp)
	movl	%eax, 4(%esp)
	leal	LC3-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L29:
	decl	-16(%ebp)
	cmpl	$-1, -16(%ebp)
	jne	L30
L31:
	leal	LC6-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	movl	$8, -16(%ebp)
	jmp	L33
L34:
	cmpl	$255, -12(%ebp)
	jg	L35
	movl	-12(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	incl	-12(%ebp)
	movl	%eax, 4(%esp)
	leal	LC3-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L33:
	decl	-16(%ebp)
	cmpl	$-1, -16(%ebp)
	jne	L34
L35:
	leal	LC6-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	movl	$8, -16(%ebp)
	jmp	L37
L38:
	cmpl	$255, -12(%ebp)
	jg	L39
	movl	-12(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	incl	-12(%ebp)
	movl	%eax, 4(%esp)
	leal	LC3-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L37:
	decl	-16(%ebp)
	cmpl	$-1, -16(%ebp)
	jne	L38
L39:
	movl	$10, (%esp)
	call	_putchar
	addl	$36, %esp
	popl	%ebx
	leave
	ret
	.cstring
LC7:
	.ascii "rom image file not available\0"
LC8:
	.ascii "reading %s with options:\12\0"
LC9:
	.ascii "-s\11single step\0"
LC10:
	.ascii "-v\11vocal\0"
LC11:
	.ascii "-l\11line per instruction\0"
LC12:
	.ascii "last load word incomplete\0"
	.align 2
LC13:
	.ascii "rom image %d target words read\12\0"
LC14:
	.ascii "rom image file required\0"
LC15:
	.ascii "async thread ID %p\12\0"
LC16:
	.ascii "g[break:point] to run\0"
LC17:
	.ascii "s to enter single step\0"
LC18:
	.ascii "key %s\12\12\0"
	.text
.globl _main
_main:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	pushl	%ebx
	subl	$240, %esp
	call	L98
"L00000000004$pb":
L98:
	popl	%ebx
	movl	12(%ebp), %eax
	movl	%eax, -204(%ebp)
	leal	L___stack_chk_guard$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %edx
	movl	%edx, -12(%ebp)
	xorl	%edx, %edx
	movl	$3000, -140(%ebp)
	movl	$1, -152(%ebp)
	movl	$0, -176(%ebp)
	leal	L_memory$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, -180(%ebp)
	movb	$0, -184(%ebp)
	movb	$0, -183(%ebp)
	movb	$0, -182(%ebp)
	movb	$0, -181(%ebp)
	movl	$0, 4(%esp)
	leal	-196(%ebp), %eax
	movl	%eax, (%esp)
	call	_gettimeofday
	movl	-196(%ebp), %eax
	imull	$1000, %eax, %esi
	movl	-192(%ebp), %ecx
	movl	$274877907, -220(%ebp)
	movl	-220(%ebp), %eax
	imull	%ecx
	sarl	$6, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	movl	%edx, %ecx
	subl	%eax, %ecx
	movl	%ecx, %eax
	leal	(%esi,%eax), %edx
	leal	_start_time-"L00000000004$pb"(%ebx), %eax
	movl	%edx, (%eax)
	jmp	L44
L45:
	movl	-152(%ebp), %eax
	sall	$2, %eax
	addl	-204(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -164(%ebp)
	movl	-164(%ebp), %eax
	movzbl	(%eax), %eax
	cmpb	$45, %al
	jne	L46
	incl	-164(%ebp)
	jmp	L48
L49:
	cmpl	$64, -160(%ebp)
	jle	L50
	cmpl	$90, -160(%ebp)
	jg	L50
	orl	$32, -160(%ebp)
L50:
	cmpl	$96, -160(%ebp)
	jle	L48
	cmpl	$122, -160(%ebp)
	jg	L48
	movl	-160(%ebp), %edx
	subl	$97, %edx
	leal	L_flag$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movb	$1, (%eax,%edx)
L48:
	movl	-164(%ebp), %eax
	movzbl	(%eax), %eax
	movsbl	%al,%eax
	movl	%eax, -160(%ebp)
	cmpl	$0, -160(%ebp)
	setne	%al
	incl	-164(%ebp)
	testb	%al, %al
	jne	L49
	jmp	L56
L46:
	leal	L_arguments$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzwl	(%eax), %eax
	cmpw	$2, %ax
	ja	L56
	leal	L_arguments$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzwl	(%eax), %ecx
	movzwl	%cx, %esi
	leal	L_argument$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	-164(%ebp), %edx
	movl	%edx, (%eax,%esi,4)
	leal	1(%ecx), %edx
	leal	L_arguments$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movw	%dx, (%eax)
L56:
	incl	-152(%ebp)
L44:
	movl	-152(%ebp), %eax
	cmpl	8(%ebp), %eax
	jl	L45
	leal	L_arguments$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzwl	(%eax), %eax
	testw	%ax, %ax
	je	L59
	leal	L_argument$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	$292, 8(%esp)
	movl	$0, 4(%esp)
	movl	%eax, (%esp)
	call	_open$UNIX2003
	movl	%eax, -168(%ebp)
	cmpl	$0, -168(%ebp)
	jns	L61
	leal	LC7-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	movl	$0, -212(%ebp)
	jmp	L63
L61:
	leal	L_flag$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	21(%eax), %eax
	testb	%al, %al
	je	L64
	leal	L_argument$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, 4(%esp)
	leal	LC8-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	leal	L_flag$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	18(%eax), %eax
	testb	%al, %al
	je	L66
	leal	LC9-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
L66:
	leal	L_flag$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	21(%eax), %eax
	testb	%al, %al
	je	L68
	leal	LC10-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
L68:
	leal	L_flag$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	11(%eax), %eax
	testb	%al, %al
	je	L64
	leal	LC11-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
L64:
	movl	$3, 8(%esp)
	leal	-184(%ebp), %eax
	incl	%eax
	movl	%eax, 4(%esp)
	movl	-168(%ebp), %eax
	movl	%eax, (%esp)
	call	_read
	movl	%eax, -172(%ebp)
	cmpl	$0, -172(%ebp)
	jle	L71
	cmpl	$3, -172(%ebp)
	je	L73
	leal	LC12-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
L73:
	movl	-180(%ebp), %edx
	movl	-184(%ebp), %eax
	movl	%eax, (%edx)
	addl	$4, -180(%ebp)
	incl	-176(%ebp)
	jmp	L64
L71:
	movl	-176(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC13-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	addl	$4095, -176(%ebp)
	movl	-176(%ebp), %eax
	sarl	$12, %eax
	movl	%eax, %edx
	leal	L_base$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, 496(%eax)
	leal	L_base$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	$127, 512(%eax)
	movl	-168(%ebp), %eax
	movl	%eax, (%esp)
	call	_close
	call	_netbank
	leal	L_arguments$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzwl	(%eax), %eax
	cmpw	$1, %ax
	ja	L75
	jmp	L76
L59:
	leal	LC14-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	movl	$0, -212(%ebp)
	jmp	L63
L75:
	leal	L_argument$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	4(%eax), %eax
	movl	%eax, (%esp)
	call	_load_fs
L76:
	leal	L_iselect$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %esi
	orl	$2, %esi
	leal	-52(%ebp), %eax
	movl	%eax, (%esp)
	call	_pthread_attr_init
	movl	%eax, %edx
	leal	L__register$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%esi,4)
	leal	L_iselect$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %esi
	orl	$2, %esi
	movl	$0, 12(%esp)
	leal	_async-"L00000000004$pb"(%ebx), %eax
	movl	%eax, 8(%esp)
	leal	-52(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	-188(%ebp), %eax
	movl	%eax, (%esp)
	call	_pthread_create
	movl	%eax, %edx
	leal	L__register$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%esi,4)
	movl	-188(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC15-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	leal	L_flag$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	18(%eax), %eax
	testb	%al, %al
	je	L77
	leal	LC16-"L00000000004$pb"(%ebx), %eax
	movl	%eax, -208(%ebp)
	jmp	L79
L77:
	leal	LC17-"L00000000004$pb"(%ebx), %edx
	movl	%edx, -208(%ebp)
L79:
	movl	-208(%ebp), %ecx
	movl	%ecx, 4(%esp)
	leal	LC18-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L80:
	leal	_runout-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	testl	%eax, %eax
	js	L81
	leal	L_apc$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %edx
	movl	(%edx), %eax
	movl	%eax, %ecx
	addl	$4, %edx
	leal	L_apc$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax)
	movl	%ecx, (%esp)
	call	_execute
	decl	-140(%ebp)
	cmpl	$0, -140(%ebp)
	jne	L83
	movl	$3000, -140(%ebp)
	movl	$0, 4(%esp)
	leal	-196(%ebp), %eax
	movl	%eax, (%esp)
	call	_gettimeofday
	movl	-196(%ebp), %eax
	imull	$1000, %eax, %esi
	movl	-192(%ebp), %ecx
	movl	$274877907, -220(%ebp)
	movl	-220(%ebp), %eax
	imull	%ecx
	sarl	$6, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	movl	%edx, %ecx
	subl	%eax, %ecx
	movl	%ecx, %eax
	leal	(%esi,%eax), %eax
	movl	%eax, -148(%ebp)
	leal	_start_time-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	subl	%eax, -148(%ebp)
	movl	-148(%ebp), %eax
	sarl	$24, %eax
	movl	%eax, %edx
	leal	L__register$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, 600(%eax)
	movl	-148(%ebp), %eax
	movl	%eax, %edx
	andl	$16777215, %edx
	leal	L__register$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, 604(%eax)
L83:
	leal	L_apc$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %edx
	leal	_breakpoint-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	cmpl	%eax, %edx
	jne	L85
	leal	L_flag$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movb	$1, 18(%eax)
L85:
	leal	L_flag$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	18(%eax), %eax
	testb	%al, %al
	je	L89
	leal	_lockstep-"L00000000004$pb"(%ebx), %eax
	movl	$1, (%eax)
	call	_statement
	movl	$62, (%esp)
	call	_putchar
	leal	L___stdoutp$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	_fflush
	jmp	L89
L90:
	movl	$10000, (%esp)
	call	_usleep
L89:
	leal	_lockstep-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	testl	%eax, %eax
	jne	L90
	leal	L_psr$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	jne	L80
	leal	L__register$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	592(%eax), %eax
	movl	%eax, -148(%ebp)
	movl	-148(%ebp), %eax
	andl	$8388608, %eax
	testl	%eax, %eax
	je	L80
	incl	-148(%ebp)
	andl	$16777215, -148(%ebp)
	movl	-148(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, 592(%eax)
	cmpl	$0, -148(%ebp)
	jne	L80
	movl	$65, (%esp)
	call	_ii
	jmp	L80
L81:
	movl	$0, -212(%ebp)
L63:
	movl	-212(%ebp), %eax
	leal	L___stack_chk_guard$non_lazy_ptr-"L00000000004$pb"(%ebx), %edx
	movl	(%edx), %edx
	movl	-12(%ebp), %ecx
	xorl	(%edx), %ecx
	je	L97
	call	___stack_chk_fail
L97:
	addl	$240, %esp
	popl	%ebx
	popl	%esi
	leave
	ret
	.cstring
LC19:
	.ascii "key s for single step\0"
LC20:
	.ascii "%x:%x\0"
LC21:
	.ascii "%x\0"
LC22:
	.ascii "%d\0"
LC23:
	.ascii "%6.6x :\0"
LC24:
	.ascii " %2.2x%2.2x%2.2x\0"
LC25:
	.ascii "%6.6x:%6.6x :\0"
LC26:
	.ascii "\12%2.2x:\0"
	.align 2
LC27:
	.ascii "\12"
	.ascii "7c                             \0"
LC28:
	.ascii "%s\0"
	.align 2
LC29:
	.ascii "LF\11execute one instruction and display new state\0"
	.align 2
LC30:
	.ascii "+\11display more internal stack registers\0"
	.align 2
LC31:
	.ascii "0n\11display registers starting at hex address\0"
	.align 2
LC32:
	.ascii "r n\11display registers starting at decimal address\0"
	.align 2
LC33:
	.ascii "b\11display relocation / configuration ports\12\0"
	.align 2
LC34:
	.ascii "m [[page:]offset]\11display system memory\0"
	.align 2
LC35:
	.ascii "\11default page[s] in current address space\12\0"
	.align 2
LC36:
	.ascii "d [[device:]offset]\11display peripheral memory array[s]\0"
	.align 2
LC37:
	.ascii "\11default device[s] in current address space\12\0"
	.align 2
LC38:
	.ascii "g [[page:]breakpoint]\11run [to breakpoint]\0"
	.align 2
LC39:
	.ascii "\11default page is current instruction section\0"
LC40:
	.ascii "\11to remove breakpoint g0:0\12\0"
LC41:
	.ascii ".\11exit emulator\0"
	.text
_action:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$436, %esp
	call	L176
"L00000000005$pb":
L176:
	popl	%ebx
	movl	8(%ebp), %eax
	movl	%eax, -412(%ebp)
	leal	L___stack_chk_guard$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %edx
	movl	%edx, -12(%ebp)
	xorl	%edx, %edx
	movl	-412(%ebp), %eax
	movzbl	(%eax), %eax
	movsbl	%al,%eax
	movl	%eax, -408(%ebp)
	leal	L_flag$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	18(%eax), %eax
	testb	%al, %al
	jne	L100
	cmpl	$115, -408(%ebp)
	je	L100
	leal	LC19-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	jmp	L174
L100:
	movl	-408(%ebp), %eax
	subl	$43, %eax
	movl	%eax, -416(%ebp)
	cmpl	$79, -416(%ebp)
	ja	L104
	movl	-416(%ebp), %edx
	sall	$2, %edx
	leal	L116-"L00000000005$pb"(%ebx), %eax
	movl	(%edx,%eax), %eax
	addl	%ebx, %eax
	jmp	*%eax
	.align 2,0x90
L116:
	.long	L105-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L106-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L107-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L108-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L109-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L110-"L00000000005$pb"
	.long	L111-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L112-"L00000000005$pb"
	.long	L113-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L107-"L00000000005$pb"
	.long	L114-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L104-"L00000000005$pb"
	.long	L115-"L00000000005$pb"
L114:
	leal	L_flag$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movb	$1, 18(%eax)
	jmp	L174
L110:
	movl	-412(%ebp), %edx
	incl	%edx
	leal	-400(%ebp), %eax
	movl	%eax, 12(%esp)
	leal	-388(%ebp), %eax
	movl	%eax, 8(%esp)
	leal	LC20-"L00000000005$pb"(%ebx), %eax
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	_sscanf
	movl	%eax, -384(%ebp)
	cmpl	$1, -384(%ebp)
	jne	L117
	leal	L_b0p$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	movl	-388(%ebp), %eax
	sall	$2, %eax
	addl	%eax, %edx
	leal	_breakpoint-"L00000000005$pb"(%ebx), %eax
	movl	%edx, (%eax)
L117:
	cmpl	$2, -384(%ebp)
	jne	L119
	movl	-388(%ebp), %eax
	movl	%eax, %edx
	sall	$14, %edx
	leal	L_memory$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	addl	%eax, %edx
	movl	-400(%ebp), %eax
	sall	$2, %eax
	addl	%eax, %edx
	leal	_breakpoint-"L00000000005$pb"(%ebx), %eax
	movl	%edx, (%eax)
L119:
	leal	L_flag$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movb	$0, 18(%eax)
	leal	_lockstep-"L00000000005$pb"(%ebx), %eax
	movl	$0, (%eax)
	movl	$58, (%esp)
	call	_putchar
	leal	L___stdoutp$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	_fflush
	jmp	L174
L105:
	leal	L_iselect$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$15, %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, -388(%ebp)
	movl	-388(%ebp), %eax
	addl	$23, %eax
	movl	%eax, -388(%ebp)
L107:
	cmpl	$43, -408(%ebp)
	je	L121
	leal	L_iselect$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	orl	$24, %eax
	movl	%eax, -388(%ebp)
L121:
	cmpl	$48, -408(%ebp)
	jne	L123
	leal	-388(%ebp), %eax
	movl	%eax, 8(%esp)
	leal	LC21-"L00000000005$pb"(%ebx), %eax
	movl	%eax, 4(%esp)
	movl	-412(%ebp), %eax
	movl	%eax, (%esp)
	call	_sscanf
	jmp	L127
L123:
	cmpl	$114, -408(%ebp)
	jne	L127
	movl	-412(%ebp), %eax
	incl	%eax
	leal	-388(%ebp), %edx
	movl	%edx, 8(%esp)
	leal	LC22-"L00000000005$pb"(%ebx), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	_sscanf
	jmp	L127
L128:
	movl	-388(%ebp), %eax
	movl	%eax, (%esp)
	call	_print_register_row
	movl	-388(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -388(%ebp)
	leal	L___stdinp$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
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
	je	L174
L127:
	movl	-388(%ebp), %eax
	cmpl	$255, %eax
	jle	L128
	jmp	L174
L113:
	movl	-412(%ebp), %edx
	incl	%edx
	leal	-400(%ebp), %eax
	movl	%eax, 12(%esp)
	leal	-396(%ebp), %eax
	movl	%eax, 8(%esp)
	leal	LC20-"L00000000005$pb"(%ebx), %eax
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	_sscanf
	movl	%eax, -384(%ebp)
	cmpl	$1, -384(%ebp)
	jg	L130
	movl	-396(%ebp), %eax
	movl	%eax, -400(%ebp)
L132:
	movl	-400(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -388(%ebp)
	movl	-400(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC23-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	jmp	L133
L134:
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
	leal	LC24-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L133:
	movl	-400(%ebp), %edx
	movl	-388(%ebp), %eax
	cmpl	%eax, %edx
	jl	L134
	leal	L___stdinp$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
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
	je	L174
	jmp	L132
L130:
	movl	-396(%ebp), %eax
	movl	%eax, %edx
	sall	$12, %edx
	movl	-400(%ebp), %eax
	leal	(%edx,%eax), %eax
	movl	%eax, -404(%ebp)
L139:
	movl	-404(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -388(%ebp)
	movl	-400(%ebp), %eax
	movl	-396(%ebp), %edx
	movl	%eax, 8(%esp)
	movl	%edx, 4(%esp)
	leal	LC25-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	movl	-400(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -400(%ebp)
	jmp	L140
L141:
	movl	-404(%ebp), %eax
	cmpl	$524287, %eax
	jg	L142
	movl	-404(%ebp), %edx
	leal	L_memory$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
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
	leal	LC24-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L140:
	movl	-404(%ebp), %edx
	movl	-388(%ebp), %eax
	cmpl	%eax, %edx
	jl	L141
L142:
	leal	L___stdinp$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
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
	je	L174
	jmp	L139
L109:
	movl	$0, -404(%ebp)
	movl	$0, -400(%ebp)
	movl	$0, -392(%ebp)
	movl	-412(%ebp), %edx
	incl	%edx
	leal	-400(%ebp), %eax
	movl	%eax, 12(%esp)
	leal	-404(%ebp), %eax
	movl	%eax, 8(%esp)
	leal	LC20-"L00000000005$pb"(%ebx), %eax
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	_sscanf
	movl	%eax, -384(%ebp)
	cmpl	$1, -384(%ebp)
	jg	L145
	movl	-404(%ebp), %eax
	movl	%eax, -400(%ebp)
	movl	$0, -404(%ebp)
	movl	-400(%ebp), %eax
	andl	$16515072, %eax
	movl	%eax, -392(%ebp)
	cmpl	$0, -392(%ebp)
	je	L147
	sarl	$18, -392(%ebp)
	movl	-400(%ebp), %eax
	andl	$262143, %eax
	movl	%eax, -400(%ebp)
	jmp	L149
L147:
	movl	-400(%ebp), %eax
	andl	$258048, %eax
	movl	%eax, -392(%ebp)
	sarl	$12, -392(%ebp)
	movl	-400(%ebp), %eax
	andl	$4095, %eax
	movl	%eax, -400(%ebp)
L149:
	cmpl	$0, -392(%ebp)
	je	L150
	cmpl	$7, -392(%ebp)
	jg	L150
	movl	-392(%ebp), %ecx
	movl	$32768, %eax
	sarl	%cl, %eax
	movl	%eax, %edx
	leal	L_psr$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	%edx, %eax
	testl	%eax, %eax
	je	L150
	addl	$64, -392(%ebp)
L150:
	movl	-392(%ebp), %edx
	leal	L_base$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, -396(%ebp)
	movl	-396(%ebp), %eax
	andl	$4194304, %eax
	testl	%eax, %eax
	je	L156
	movl	-396(%ebp), %eax
	andl	$63, %eax
	movl	%eax, -404(%ebp)
	jmp	L156
L145:
	movl	-404(%ebp), %eax
	orl	$4194304, %eax
	movl	%eax, -396(%ebp)
L156:
	movl	-400(%ebp), %eax
	movl	-396(%ebp), %edx
	movl	%eax, 8(%esp)
	movl	%edx, 4(%esp)
	leal	LC25-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	movl	-400(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -388(%ebp)
	jmp	L157
L158:
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
	leal	LC3-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L157:
	movl	-400(%ebp), %edx
	movl	-388(%ebp), %eax
	cmpl	%eax, %edx
	jl	L158
	leal	L___stdinp$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
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
	je	L174
	jmp	L156
L108:
	movl	$0, -384(%ebp)
	jmp	L162
L163:
	movl	-384(%ebp), %eax
	andl	$7, %eax
	testl	%eax, %eax
	jne	L164
	movl	-384(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC26-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L164:
	movl	-384(%ebp), %edx
	leal	L_base$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, 4(%esp)
	leal	LC3-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	incl	-384(%ebp)
L162:
	cmpl	$71, -384(%ebp)
	jle	L163
	leal	LC27-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	movl	$124, -384(%ebp)
	jmp	L167
L168:
	movl	-384(%ebp), %eax
	andl	$7, %eax
	testl	%eax, %eax
	jne	L169
	movl	-384(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC26-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L169:
	movl	-384(%ebp), %edx
	leal	L_base$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, 4(%esp)
	leal	LC3-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	incl	-384(%ebp)
L167:
	cmpl	$191, -384(%ebp)
	jle	L168
	movl	$10, (%esp)
	call	_putchar
	jmp	L174
L115:
	movl	-412(%ebp), %eax
	incl	%eax
	leal	_start_time-"L00000000005$pb"(%ebx), %edx
	movl	%edx, 8(%esp)
	leal	LC22-"L00000000005$pb"(%ebx), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	_sscanf
	jmp	L174
L112:
	movl	-412(%ebp), %edx
	incl	%edx
	leal	-372(%ebp), %eax
	movl	%eax, 8(%esp)
	leal	LC28-"L00000000005$pb"(%ebx), %eax
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	_sscanf
	movl	%eax, -384(%ebp)
	cmpl	$1, -384(%ebp)
	jne	L174
	leal	-372(%ebp), %eax
	movl	%eax, (%esp)
	call	_load_fs
	jmp	L174
L111:
	leal	LC29-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	leal	LC30-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	leal	LC31-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	leal	LC32-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	leal	LC33-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	leal	LC34-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	leal	LC35-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	leal	LC36-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	leal	LC37-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	leal	LC38-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	leal	LC39-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	leal	LC40-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	leal	LC41-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	jmp	L174
L106:
	leal	_runout-"L00000000005$pb"(%ebx), %eax
	movl	$-1, (%eax)
	leal	_lockstep-"L00000000005$pb"(%ebx), %eax
	movl	$0, (%eax)
	jmp	L174
L104:
	leal	_lockstep-"L00000000005$pb"(%ebx), %eax
	movl	$0, (%eax)
L174:
	leal	L___stack_chk_guard$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	-12(%ebp), %edx
	xorl	(%eax), %edx
	je	L175
	call	___stack_chk_fail
L175:
	addl	$436, %esp
	popl	%ebx
	leave
	ret
	.cstring
	.align 2
LC42:
	.ascii "file system image %s error %d\12\0"
LC43:
	.ascii "loading file system image %s\12\0"
	.align 2
LC44:
	.ascii "volume header read unsuccessful\0"
LC45:
	.ascii "[%2.2x%2.2x%2.2x]\12\0"
LC46:
	.ascii "\12%4.4x: \0"
LC47:
	.ascii "%2.2x\0"
LC48:
	.ascii "%.*s %d storage banks\12\0"
	.align 2
LC49:
	.ascii "%d unable to cache file system\12\0"
LC50:
	.ascii "%d granules loaded\12\0"
	.text
_load_fs:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$3124, %esp
	call	L207
"L00000000006$pb":
L207:
	popl	%ebx
	movl	$0, -20(%ebp)
	leal	L_devices$non_lazy_ptr-"L00000000006$pb"(%ebx), %eax
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
	jns	L178
	call	___error
	movl	(%eax), %eax
	movl	%eax, 8(%esp)
	movl	8(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC42-"L00000000006$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	jmp	L206
L178:
	movl	8(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC43-"L00000000006$pb"(%ebx), %eax
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
	je	L181
	leal	LC44-"L00000000006$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	jmp	L206
L181:
	leal	L_flag$non_lazy_ptr-"L00000000006$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	21(%eax), %eax
	testb	%al, %al
	je	L183
	movzbl	-3053(%ebp), %eax
	movzbl	%al, %edx
	movzbl	-3054(%ebp), %eax
	movzbl	%al, %ecx
	movzbl	-3055(%ebp), %eax
	movzbl	%al, %eax
	movl	%edx, 12(%esp)
	movl	%ecx, 8(%esp)
	movl	%eax, 4(%esp)
	leal	LC45-"L00000000006$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L183:
	movzbl	-3055(%ebp), %eax
	cmpb	$86, %al
	je	L185
	leal	L_flag$non_lazy_ptr-"L00000000006$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	21(%eax), %eax
	testb	%al, %al
	je	L206
	movl	$0, -20(%ebp)
	leal	-3100(%ebp), %eax
	movl	%eax, -24(%ebp)
	jmp	L189
L190:
	movl	-20(%ebp), %eax
	andl	$15, %eax
	testl	%eax, %eax
	jne	L191
	movl	-20(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC46-"L00000000006$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L191:
	movl	-24(%ebp), %eax
	movzbl	(%eax), %eax
	movsbl	%al,%eax
	incl	-24(%ebp)
	movl	%eax, 4(%esp)
	leal	LC47-"L00000000006$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	incl	-20(%ebp)
L189:
	decl	-12(%ebp)
	cmpl	$-1, -12(%ebp)
	jne	L190
	movl	$10, (%esp)
	call	_putchar
	jmp	L206
L185:
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
	leal	LC48-"L00000000006$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	cmpl	$0, -16(%ebp)
	je	L206
	cmpl	$0, -24(%ebp)
	je	L195
	movl	-24(%ebp), %eax
	movl	%eax, (%esp)
	call	_free
L195:
	movl	-16(%ebp), %eax
	movl	$786432, 4(%esp)
	movl	%eax, (%esp)
	call	_calloc
	movl	%eax, -24(%ebp)
	cmpl	$0, -24(%ebp)
	jne	L197
	call	___error
	movl	(%eax), %eax
	movl	%eax, 4(%esp)
	leal	LC49-"L00000000006$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	jmp	L206
L197:
	movl	-24(%ebp), %edx
	leal	L_devices$non_lazy_ptr-"L00000000006$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, 12(%eax)
	movl	-16(%ebp), %eax
	orl	$14680064, %eax
	movl	%eax, %edx
	leal	L_base$non_lazy_ptr-"L00000000006$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, 516(%eax)
	movl	$-1, %eax
	cmpl	$-1, %eax
	je	L199
	movl	$-1, %eax
	movl	%eax, 12(%esp)
	movl	$3072, 8(%esp)
	leal	-3100(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	-24(%ebp), %eax
	movl	%eax, (%esp)
	call	___memcpy_chk
	jmp	L201
L199:
	movl	$3072, 8(%esp)
	leal	-3100(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	-24(%ebp), %eax
	movl	%eax, (%esp)
	call	___inline_memcpy_chk
L201:
	addl	$3072, -24(%ebp)
	movl	$16, -20(%ebp)
L202:
	movl	$192, 8(%esp)
	movl	-24(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	-28(%ebp), %eax
	movl	%eax, (%esp)
	call	_read
	movl	%eax, -12(%ebp)
	cmpl	$0, -12(%ebp)
	js	L203
	cmpl	$0, -12(%ebp)
	je	L203
	incl	-20(%ebp)
	addl	$192, -24(%ebp)
	jmp	L202
L203:
	movl	-28(%ebp), %eax
	movl	%eax, (%esp)
	call	_close
	movl	-20(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC50-"L00000000006$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L206:
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
.lcomm _breakpoint,4,2
.lcomm _runout,4,2
.lcomm _lockstep,4,2
.lcomm _start_time,4,2
.comm _flag,26,0
.comm _arguments,2,1
.comm _argument,12,2
	.section __IMPORT,__pointers,non_lazy_symbol_pointers
L_b0p$non_lazy_ptr:
	.indirect_symbol _b0p
	.long	0
L___stdoutp$non_lazy_ptr:
	.indirect_symbol ___stdoutp
	.long	0
L_apc$non_lazy_ptr:
	.indirect_symbol _apc
	.long	0
L_memory$non_lazy_ptr:
	.indirect_symbol _memory
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
L_psr$non_lazy_ptr:
	.indirect_symbol _psr
	.long	0
L_iselect$non_lazy_ptr:
	.indirect_symbol _iselect
	.long	0
L_flag$non_lazy_ptr:
	.indirect_symbol _flag
	.long	0
L_base$non_lazy_ptr:
	.indirect_symbol _base
	.long	0
L_argument$non_lazy_ptr:
	.indirect_symbol _argument
	.long	0
L___stdinp$non_lazy_ptr:
	.indirect_symbol ___stdinp
	.long	0
	.subsections_via_symbols
