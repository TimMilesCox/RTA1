	.globl	_trample
_trample:
	movbeq	(%rdi), %rax
	movbeq	%rax, (%rsi)
	retq

