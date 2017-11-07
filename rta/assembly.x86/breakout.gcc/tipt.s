	.data
	.align 2
_attention:
	.long	0
	.word	1
	.space 2
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
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$220, %esp
	call	L32
"L00000000001$pb":
L32:
	popl	%ebx
	movl	12(%ebp), %eax
	movl	%eax, -188(%ebp)
	leal	L___stack_chk_guard$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %edx
	movl	%edx, -28(%ebp)
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
	movl	$274877907, -204(%ebp)
	movl	-204(%ebp), %eax
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
	movl	$0, -164(%ebp)
	leal	L_memory$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, -168(%ebp)
	movb	$0, -172(%ebp)
	movb	$0, -171(%ebp)
	movb	$0, -170(%ebp)
	movb	$0, -169(%ebp)
	movl	-188(%ebp), %eax
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
	movl	%eax, -156(%ebp)
	cmpl	$0, -156(%ebp)
	jns	L4
	leal	LC0-"L00000000001$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	movl	$0, -196(%ebp)
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
	leal	-172(%ebp), %eax
	incl	%eax
	movl	%eax, 4(%esp)
	movl	-156(%ebp), %eax
	movl	%eax, (%esp)
	call	_read
	movl	%eax, -160(%ebp)
	cmpl	$0, -160(%ebp)
	jle	L14
	cmpl	$3, -160(%ebp)
	je	L16
	leal	LC5-"L00000000001$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
L16:
	movl	-168(%ebp), %edx
	movl	-172(%ebp), %eax
	movl	%eax, (%edx)
	addl	$4, -168(%ebp)
	incl	-164(%ebp)
	jmp	L7
L14:
	movl	-164(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC6-"L00000000001$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	addl	$4095, -164(%ebp)
	movl	-164(%ebp), %eax
	sarl	$12, %eax
	movl	%eax, %edx
	leal	L_base$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, 496(%eax)
	leal	L_base$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	$127, 512(%eax)
	movl	-156(%ebp), %eax
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
	movl	$0, -196(%ebp)
	jmp	L6
L18:
	leal	L_argument$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	4(%eax), %eax
	movl	%eax, (%esp)
	call	_load_fs
L19:
	leal	L_iselect$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %esi
	orl	$2, %esi
	leal	-68(%ebp), %eax
	movl	%eax, (%esp)
	call	_pthread_attr_init
	movl	%eax, %edx
	leal	L__register$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%esi,4)
	leal	L_iselect$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %esi
	orl	$2, %esi
	movl	$0, 12(%esp)
	leal	_emulate-"L00000000001$pb"(%ebx), %eax
	movl	%eax, 8(%esp)
	leal	-68(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	-152(%ebp), %eax
	movl	%eax, (%esp)
	call	_pthread_create
	movl	%eax, %edx
	leal	L__register$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax,%esi,4)
	movl	-152(%ebp), %eax
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
	movl	%eax, -192(%ebp)
	jmp	L22
L20:
	leal	LC10-"L00000000001$pb"(%ebx), %edx
	movl	%edx, -192(%ebp)
L22:
	movl	-192(%ebp), %ecx
	movl	%ecx, 4(%esp)
	leal	LC11-"L00000000001$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L23:
	leal	L_iselect$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
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
	leal	L_iselect$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
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
	leal	-140(%ebp), %eax
	movl	%eax, (%esp)
	call	_fgets
	movl	%eax, -148(%ebp)
	cmpl	$0, -148(%ebp)
	jne	L26
	leal	LC12-"L00000000001$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	jmp	L28
L26:
	movl	-148(%ebp), %eax
	movzbl	(%eax), %eax
	cmpb	$46, %al
	je	L28
	movl	-148(%ebp), %eax
	movl	%eax, (%esp)
	call	_action
L24:
	movl	$0, 4(%esp)
	leal	_time1-"L00000000001$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_gettimeofday
	leal	_u-"L00000000001$pb"(%ebx), %edi
	leal	_time1-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$1000, %eax, %esi
	leal	_time1-"L00000000001$pb"(%ebx), %eax
	movl	4(%eax), %ecx
	movl	$274877907, -204(%ebp)
	movl	-204(%ebp), %eax
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
	movl	%eax, (%edi)
	movl	%edx, 4(%edi)
	leal	_indication-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$1, %edx
	leal	_indication-"L00000000001$pb"(%ebx), %eax
	movl	%edx, (%eax)
	jmp	L23
L28:
	movl	$0, -196(%ebp)
L6:
	movl	-196(%ebp), %eax
	leal	L___stack_chk_guard$non_lazy_ptr-"L00000000001$pb"(%ebx), %edx
	movl	(%edx), %edx
	movl	-28(%ebp), %ecx
	xorl	(%edx), %ecx
	je	L31
	call	___stack_chk_fail
L31:
	addl	$220, %esp
	popl	%ebx
	popl	%esi
	popl	%edi
	leave
	ret
_emulate:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$28, %esp
	call	L43
"L00000000002$pb":
L43:
	popl	%ebx
L34:
	L35: # LASM$engage
	# top of block
	.file "tipt.c"
	.line 262
	leal	L_register_set$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl (%eax), %ebp
	.file "tipt.c"
	.line 263
	leal	L_apc$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl (%eax), %edx
	L36: # LASM$next
	.file "tipt.c"
	.line 265
	movl 0(%edx), %eax
	.file "tipt.c"
	.line 266
	addl $4, %edx
	.file "tipt.c"
	.line 267
	bswapl %eax
	.file "tipt.c"
	.line 268
	call _execute
	.file "tipt.c"
	.line 269
	testl $7, _indication
	.file "tipt.c"
	.line 270
	jz L36
	.file "tipt.c"
	.line 272
	testl $1, _indication
	.file "tipt.c"
	.line 273
	jz L37
	.file "tipt.c"
	.line 275
	leal	_u-"L00000000002$pb"(%ebx), %eax
	movl (%eax), %ebx
	.file "tipt.c"
	.line 276
	leal	_u-"L00000000002$pb"(%ebx), %eax
	movl 4(%eax), %eax
	.file "tipt.c"
	.line 277
	bswapl %ebx
	.file "tipt.c"
	.line 278
	shll $8, %eax
	.file "tipt.c"
	.line 279
	movb %bl, %al
	.file "tipt.c"
	.line 280
	xorb %bl, %bl
	.file "tipt.c"
	.line 281
	leal	L__register$non_lazy_ptr-"L00000000002$pb"(%ebx), %ecx
	movl	(%ecx), %ecx
	movl %ebx, 604(%ecx)
	.file "tipt.c"
	.line 282
	bswapl %eax
	.file "tipt.c"
	.line 283
	xorb %al, %al
	.file "tipt.c"
	.line 284
	leal	L__register$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl %ebx, 600(%eax)
	L37: # LASM$ifbreakpoint
	.file "tipt.c"
	.line 287
	xorb %bl, %bl
	.file "tipt.c"
	.line 288
	testl $4, _indication
	.file "tipt.c"
	.line 289
	jz L38
	.file "tipt.c"
	.line 290
	cmpl _breakpoint, %edx
	.file "tipt.c"
	.line 291
	jnz L38
	.file "tipt.c"
	.line 292
	leal	L_flag$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movb $1, 18(%eax)
	.file "tipt.c"
	.line 293
	orl $2, _indication
	L38: # LASM$ifconsole
	.file "tipt.c"
	.line 296
	testl $2, _indication
	.file "tipt.c"
	.line 297
	jz L36
	.file "tipt.c"
	.line 298
	leal	L_register_set$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl %ebp, (%eax)
	.file "tipt.c"
	.line 299
	leal	L_apc$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl %edx, (%eax)
	call	_statement
	movl	$62, (%esp)
	call	_putchar
	leal	L___stdoutp$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	_fflush
	jmp	L39
L40:
	movl	$10000, (%esp)
	call	_usleep
L39:
	leal	_indication-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	andl	$2, %eax
	testl	%eax, %eax
	jne	L40
	jmp	L34
	.cstring
LC13:
	.ascii "key s for single step\0"
LC14:
	.ascii "%x:%x\0"
LC15:
	.ascii "%x\0"
LC16:
	.ascii "%d\0"
LC17:
	.ascii "%6.6x :\0"
LC18:
	.ascii " %2.2x%2.2x%2.2x\0"
LC19:
	.ascii "%6.6x:%6.6x :\0"
LC20:
	.ascii " %6.6x\0"
LC21:
	.ascii "\12%2.2x:\0"
	.align 2
LC22:
	.ascii "\12"
	.ascii "7c                             \0"
LC23:
	.ascii "%lld\0"
LC24:
	.ascii "%s\0"
	.align 2
LC25:
	.ascii "LF\11execute one instruction and display new state\0"
	.align 2
LC26:
	.ascii "+\11display more internal stack registers\0"
	.align 2
LC27:
	.ascii "0n\11display registers starting at hex address\0"
	.align 2
LC28:
	.ascii "r n\11display registers starting at decimal address\0"
	.align 2
LC29:
	.ascii "b\11display relocation / configuration ports\12\0"
	.align 2
LC30:
	.ascii "m [[page:]offset]\11display system memory\0"
	.align 2
LC31:
	.ascii "\11default page[s] in current address space\12\0"
	.align 2
LC32:
	.ascii "d [[device:]offset]\11display peripheral memory array[s]\0"
	.align 2
LC33:
	.ascii "\11default device[s] in current address space\12\0"
	.align 2
LC34:
	.ascii "g [[page:]breakpoint]\11run [to breakpoint]\0"
	.align 2
LC35:
	.ascii "\11default page is current instruction section\0"
LC36:
	.ascii "\11to remove breakpoint g0:0\12\0"
LC37:
	.ascii ".\11exit emulator\0"
	.text
_action:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$436, %esp
	call	L121
"L00000000003$pb":
L121:
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
	jne	L45
	cmpl	$115, -408(%ebp)
	je	L45
	leal	LC13-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	jmp	L119
L45:
	movl	-408(%ebp), %eax
	subl	$43, %eax
	movl	%eax, -416(%ebp)
	cmpl	$79, -416(%ebp)
	ja	L49
	movl	-416(%ebp), %edx
	sall	$2, %edx
	leal	L61-"L00000000003$pb"(%ebx), %eax
	movl	(%edx,%eax), %eax
	addl	%ebx, %eax
	jmp	*%eax
	.align 2,0x90
L61:
	.long	L50-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L51-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L52-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L53-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L54-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L55-"L00000000003$pb"
	.long	L56-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L57-"L00000000003$pb"
	.long	L58-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L52-"L00000000003$pb"
	.long	L59-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L49-"L00000000003$pb"
	.long	L60-"L00000000003$pb"
L59:
	leal	L_flag$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movb	$1, 18(%eax)
	jmp	L119
L55:
	movl	-412(%ebp), %edx
	incl	%edx
	leal	-400(%ebp), %eax
	movl	%eax, 12(%esp)
	leal	-388(%ebp), %eax
	movl	%eax, 8(%esp)
	leal	LC14-"L00000000003$pb"(%ebx), %eax
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	_sscanf
	movl	%eax, -384(%ebp)
	cmpl	$1, -384(%ebp)
	jne	L62
	leal	L_b0p$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	movl	-388(%ebp), %eax
	sall	$2, %eax
	addl	%eax, %edx
	leal	_breakpoint-"L00000000003$pb"(%ebx), %eax
	movl	%edx, (%eax)
L62:
	cmpl	$2, -384(%ebp)
	jne	L64
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
L64:
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
	jmp	L119
L50:
	leal	L_iselect$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
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
L52:
	cmpl	$43, -408(%ebp)
	je	L66
	leal	L_iselect$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	orl	$24, %eax
	movl	%eax, -388(%ebp)
L66:
	cmpl	$48, -408(%ebp)
	jne	L68
	leal	-388(%ebp), %eax
	movl	%eax, 8(%esp)
	leal	LC15-"L00000000003$pb"(%ebx), %eax
	movl	%eax, 4(%esp)
	movl	-412(%ebp), %eax
	movl	%eax, (%esp)
	call	_sscanf
	jmp	L72
L68:
	cmpl	$114, -408(%ebp)
	jne	L72
	movl	-412(%ebp), %eax
	incl	%eax
	leal	-388(%ebp), %edx
	movl	%edx, 8(%esp)
	leal	LC16-"L00000000003$pb"(%ebx), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	_sscanf
	jmp	L72
L73:
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
	je	L119
L72:
	movl	-388(%ebp), %eax
	cmpl	$255, %eax
	jle	L73
	jmp	L119
L58:
	movl	-412(%ebp), %edx
	incl	%edx
	leal	-400(%ebp), %eax
	movl	%eax, 12(%esp)
	leal	-396(%ebp), %eax
	movl	%eax, 8(%esp)
	leal	LC14-"L00000000003$pb"(%ebx), %eax
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	_sscanf
	movl	%eax, -384(%ebp)
	cmpl	$1, -384(%ebp)
	jg	L75
	movl	-396(%ebp), %eax
	movl	%eax, -400(%ebp)
L77:
	movl	-400(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -388(%ebp)
	movl	-400(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC17-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	jmp	L78
L79:
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
	leal	LC18-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L78:
	movl	-400(%ebp), %edx
	movl	-388(%ebp), %eax
	cmpl	%eax, %edx
	jl	L79
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
	je	L119
	jmp	L77
L75:
	movl	-396(%ebp), %eax
	movl	%eax, %edx
	sall	$12, %edx
	movl	-400(%ebp), %eax
	leal	(%edx,%eax), %eax
	movl	%eax, -404(%ebp)
L84:
	movl	-404(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -388(%ebp)
	movl	-400(%ebp), %eax
	movl	-396(%ebp), %edx
	movl	%eax, 8(%esp)
	movl	%edx, 4(%esp)
	leal	LC19-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	movl	-400(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -400(%ebp)
	jmp	L85
L86:
	movl	-404(%ebp), %eax
	cmpl	$524287, %eax
	jg	L87
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
	leal	LC18-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L85:
	movl	-404(%ebp), %edx
	movl	-388(%ebp), %eax
	cmpl	%eax, %edx
	jl	L86
L87:
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
	je	L119
	jmp	L84
L54:
	movl	$0, -404(%ebp)
	movl	$0, -400(%ebp)
	movl	$0, -392(%ebp)
	movl	-412(%ebp), %edx
	incl	%edx
	leal	-400(%ebp), %eax
	movl	%eax, 12(%esp)
	leal	-404(%ebp), %eax
	movl	%eax, 8(%esp)
	leal	LC14-"L00000000003$pb"(%ebx), %eax
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	_sscanf
	movl	%eax, -384(%ebp)
	cmpl	$1, -384(%ebp)
	jg	L90
	movl	-404(%ebp), %eax
	movl	%eax, -400(%ebp)
	movl	$0, -404(%ebp)
	movl	-400(%ebp), %eax
	andl	$16515072, %eax
	movl	%eax, -392(%ebp)
	cmpl	$0, -392(%ebp)
	je	L92
	sarl	$18, -392(%ebp)
	movl	-400(%ebp), %eax
	andl	$262143, %eax
	movl	%eax, -400(%ebp)
	jmp	L94
L92:
	movl	-400(%ebp), %eax
	andl	$258048, %eax
	movl	%eax, -392(%ebp)
	sarl	$12, -392(%ebp)
	movl	-400(%ebp), %eax
	andl	$4095, %eax
	movl	%eax, -400(%ebp)
L94:
	cmpl	$0, -392(%ebp)
	je	L95
	cmpl	$7, -392(%ebp)
	jg	L95
	movl	-392(%ebp), %ecx
	movl	$32768, %eax
	sarl	%cl, %eax
	movl	%eax, %edx
	leal	L_psr$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	andl	%edx, %eax
	testl	%eax, %eax
	je	L95
	addl	$64, -392(%ebp)
L95:
	movl	-392(%ebp), %edx
	leal	L_base$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, -396(%ebp)
	movl	-396(%ebp), %eax
	andl	$4194304, %eax
	testl	%eax, %eax
	je	L101
	movl	-396(%ebp), %eax
	andl	$63, %eax
	movl	%eax, -404(%ebp)
	jmp	L101
L90:
	movl	-404(%ebp), %eax
	orl	$4194304, %eax
	movl	%eax, -396(%ebp)
L101:
	movl	-400(%ebp), %eax
	movl	-396(%ebp), %edx
	movl	%eax, 8(%esp)
	movl	%edx, 4(%esp)
	leal	LC19-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	movl	-400(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -388(%ebp)
	jmp	L102
L103:
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
	leal	LC20-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L102:
	movl	-400(%ebp), %edx
	movl	-388(%ebp), %eax
	cmpl	%eax, %edx
	jl	L103
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
	je	L119
	jmp	L101
L53:
	movl	$0, -384(%ebp)
	jmp	L107
L108:
	movl	-384(%ebp), %eax
	andl	$7, %eax
	testl	%eax, %eax
	jne	L109
	movl	-384(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC21-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L109:
	movl	-384(%ebp), %edx
	leal	L_base$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, 4(%esp)
	leal	LC20-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	incl	-384(%ebp)
L107:
	cmpl	$71, -384(%ebp)
	jle	L108
	leal	LC22-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	movl	$124, -384(%ebp)
	jmp	L112
L113:
	movl	-384(%ebp), %eax
	andl	$7, %eax
	testl	%eax, %eax
	jne	L114
	movl	-384(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC21-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L114:
	movl	-384(%ebp), %edx
	leal	L_base$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, 4(%esp)
	leal	LC20-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	incl	-384(%ebp)
L112:
	cmpl	$191, -384(%ebp)
	jle	L113
	movl	$10, (%esp)
	call	_putchar
	jmp	L119
L60:
	movl	-412(%ebp), %eax
	incl	%eax
	leal	_start_time-"L00000000003$pb"(%ebx), %edx
	movl	%edx, 8(%esp)
	leal	LC23-"L00000000003$pb"(%ebx), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	_sscanf
	jmp	L119
L57:
	movl	-412(%ebp), %edx
	incl	%edx
	leal	-372(%ebp), %eax
	movl	%eax, 8(%esp)
	leal	LC24-"L00000000003$pb"(%ebx), %eax
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	call	_sscanf
	movl	%eax, -384(%ebp)
	cmpl	$1, -384(%ebp)
	jne	L119
	leal	-372(%ebp), %eax
	movl	%eax, (%esp)
	call	_load_fs
	jmp	L119
L56:
	leal	LC25-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	leal	LC26-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	leal	LC27-"L00000000003$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
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
	jmp	L119
L51:
	leal	_indication-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	andl	$-3, %edx
	leal	_indication-"L00000000003$pb"(%ebx), %eax
	movl	%edx, (%eax)
	jmp	L119
L49:
	leal	_indication-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	andl	$-3, %edx
	leal	_indication-"L00000000003$pb"(%ebx), %eax
	movl	%edx, (%eax)
L119:
	leal	L___stack_chk_guard$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	-12(%ebp), %edx
	xorl	(%eax), %edx
	je	L120
	call	___stack_chk_fail
L120:
	addl	$436, %esp
	popl	%ebx
	leave
	ret
	.cstring
	.align 2
LC38:
	.ascii "file system image %s error %d\12\0"
LC39:
	.ascii "loading file system image %s\12\0"
	.align 2
LC40:
	.ascii "volume header read unsuccessful\0"
LC41:
	.ascii "[%2.2x%2.2x%2.2x]\12\0"
LC42:
	.ascii "\12%4.4x: \0"
LC43:
	.ascii "%2.2x\0"
LC44:
	.ascii "%.*s %d storage banks\12\0"
	.align 2
LC45:
	.ascii "%d unable to cache file system\12\0"
LC46:
	.ascii "%d granules loaded\12\0"
	.text
_load_fs:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$3124, %esp
	call	L152
"L00000000004$pb":
L152:
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
	jns	L123
	call	___error
	movl	(%eax), %eax
	movl	%eax, 8(%esp)
	movl	8(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC38-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	jmp	L151
L123:
	movl	8(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC39-"L00000000004$pb"(%ebx), %eax
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
	je	L126
	leal	LC40-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	jmp	L151
L126:
	leal	L_flag$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	21(%eax), %eax
	testb	%al, %al
	je	L128
	movzbl	-3053(%ebp), %eax
	movzbl	%al, %edx
	movzbl	-3054(%ebp), %eax
	movzbl	%al, %ecx
	movzbl	-3055(%ebp), %eax
	movzbl	%al, %eax
	movl	%edx, 12(%esp)
	movl	%ecx, 8(%esp)
	movl	%eax, 4(%esp)
	leal	LC41-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L128:
	movzbl	-3055(%ebp), %eax
	cmpb	$86, %al
	je	L130
	leal	L_flag$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	21(%eax), %eax
	testb	%al, %al
	je	L151
	movl	$0, -20(%ebp)
	leal	-3100(%ebp), %eax
	movl	%eax, -24(%ebp)
	jmp	L134
L135:
	movl	-20(%ebp), %eax
	andl	$15, %eax
	testl	%eax, %eax
	jne	L136
	movl	-20(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC42-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L136:
	movl	-24(%ebp), %eax
	movzbl	(%eax), %eax
	movsbl	%al,%eax
	incl	-24(%ebp)
	movl	%eax, 4(%esp)
	leal	LC43-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	incl	-20(%ebp)
L134:
	decl	-12(%ebp)
	cmpl	$-1, -12(%ebp)
	jne	L135
	movl	$10, (%esp)
	call	_putchar
	jmp	L151
L130:
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
	leal	LC44-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	cmpl	$0, -16(%ebp)
	je	L151
	cmpl	$0, -24(%ebp)
	je	L140
	movl	-24(%ebp), %eax
	movl	%eax, (%esp)
	call	_free
L140:
	movl	-16(%ebp), %eax
	movl	$786432, 4(%esp)
	movl	%eax, (%esp)
	call	_calloc
	movl	%eax, -24(%ebp)
	cmpl	$0, -24(%ebp)
	jne	L142
	call	___error
	movl	(%eax), %eax
	movl	%eax, 4(%esp)
	leal	LC45-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	jmp	L151
L142:
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
	je	L144
	movl	$-1, %eax
	movl	%eax, 12(%esp)
	movl	$3072, 8(%esp)
	leal	-3100(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	-24(%ebp), %eax
	movl	%eax, (%esp)
	call	___memcpy_chk
	jmp	L146
L144:
	movl	$3072, 8(%esp)
	leal	-3100(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	-24(%ebp), %eax
	movl	%eax, (%esp)
	call	___inline_memcpy_chk
L146:
	addl	$3072, -24(%ebp)
	movl	$16, -20(%ebp)
L147:
	movl	$192, 8(%esp)
	movl	-24(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	-28(%ebp), %eax
	movl	%eax, (%esp)
	call	_read
	movl	%eax, -12(%ebp)
	cmpl	$0, -12(%ebp)
	js	L148
	cmpl	$0, -12(%ebp)
	je	L148
	incl	-20(%ebp)
	addl	$192, -24(%ebp)
	jmp	L147
L148:
	movl	-28(%ebp), %eax
	movl	%eax, (%esp)
	call	_close
	movl	-20(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC46-"L00000000004$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L151:
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
LC47:
	.ascii "%6.6x %8.8x\12\0"
LC48:
	.ascii "[%6.6x]->\0"
LC49:
	.ascii "\12   \0"
	.text
_statement:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$36, %esp
	call	L172
"L00000000005$pb":
L172:
	popl	%ebx
	leal	L_iselect$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, -12(%ebp)
	leal	L_flag$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	11(%eax), %eax
	movsbl	%al,%edx
	leal	L_apc$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	subl	$4, %eax
	movl	%edx, 8(%esp)
	movl	$1, 4(%esp)
	movl	%eax, (%esp)
	call	_instruction_display
	leal	L_apc$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	leal	L_memory$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, %ecx
	subl	%eax, %ecx
	movl	%ecx, %eax
	sarl	$2, %eax
	movl	%eax, %edx
	leal	L_psr$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%edx, 8(%esp)
	movl	%eax, 4(%esp)
	leal	LC47-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	leal	L_flag$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	11(%eax), %eax
	movsbl	%al,%edx
	leal	L_apc$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%edx, 8(%esp)
	movl	$6, 4(%esp)
	movl	%eax, (%esp)
	call	_instruction_display
	jmp	L156
L157:
	movl	-12(%ebp), %eax
	movl	%eax, (%esp)
	call	_print_register_row
	movl	$10, (%esp)
	call	_putchar
	addl	$8, -12(%ebp)
L156:
	leal	L_iselect$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	orl	$24, %eax
	cmpl	-12(%ebp), %eax
	jg	L157
	leal	L_iselect$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, %edx
	orl	$15, %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC48-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	movl	$7, -16(%ebp)
	jmp	L159
L160:
	cmpl	$255, -12(%ebp)
	jg	L161
	movl	-12(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	incl	-12(%ebp)
	movl	%eax, 4(%esp)
	leal	LC20-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L159:
	decl	-16(%ebp)
	cmpl	$-1, -16(%ebp)
	jne	L160
L161:
	leal	LC49-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	movl	$8, -16(%ebp)
	jmp	L163
L164:
	cmpl	$255, -12(%ebp)
	jg	L165
	movl	-12(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	incl	-12(%ebp)
	movl	%eax, 4(%esp)
	leal	LC20-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L163:
	decl	-16(%ebp)
	cmpl	$-1, -16(%ebp)
	jne	L164
L165:
	leal	LC49-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	movl	$8, -16(%ebp)
	jmp	L167
L168:
	cmpl	$255, -12(%ebp)
	jg	L169
	movl	-12(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	incl	-12(%ebp)
	movl	%eax, 4(%esp)
	leal	LC20-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L167:
	decl	-16(%ebp)
	cmpl	$-1, -16(%ebp)
	jne	L168
L169:
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
LC50:
	.ascii "%2.2x:\0"
	.text
_print_register_row:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$36, %esp
	call	L181
"L00000000006$pb":
L181:
	popl	%ebx
	movl	$8, -12(%ebp)
	movl	8(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC50-"L00000000006$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	jmp	L176
L177:
	movl	8(%ebp), %edx
	leal	L__register$non_lazy_ptr-"L00000000006$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax,%edx,4), %eax
	incl	8(%ebp)
	movl	%eax, 4(%esp)
	leal	LC20-"L00000000006$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L176:
	decl	-12(%ebp)
	cmpl	$-1, -12(%ebp)
	je	L180
	cmpl	$255, 8(%ebp)
	jle	L177
L180:
	addl	$36, %esp
	popl	%ebx
	leave
	ret
.lcomm _breakpoint,4,2
.lcomm _indication,4,2
.lcomm _start_time,8,3
.lcomm _u,8,2
.lcomm _time1,8,2
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
L_register_set$non_lazy_ptr:
	.indirect_symbol _register_set
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
