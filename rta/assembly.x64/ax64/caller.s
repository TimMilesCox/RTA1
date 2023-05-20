	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 11
	.globl	_main
	.align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp0:
	.cfi_def_cfa_offset 16
Ltmp1:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp2:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	leaq	_sample(%rip), %rax
	addq	$4, %rax
	leaq	_expect(%rip), %rsi
	movl	$0, -4(%rbp)
	movq	%rax, %rdi
	callq	_trample
	leaq	L_.str(%rip), %rdi
	leaq	_expect(%rip), %rdx
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rsi
	movb	$0, %al
	callq	_printf
	xorl	%ecx, %ecx
	movl	%eax, -20(%rbp)         ## 4-byte Spill
	movl	%ecx, %eax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.section	__DATA,__data
	.align	2                       ## @sample
_sample:
	.long	0                       ## 0x0
	.long	1634037065              ## 0x61656d49
	.long	175401326               ## 0xa74696e

_expect:                                ## @expect
	.asciz	"////////"

	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"%lx %s\n"


.subsections_via_symbols
