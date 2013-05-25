	.section	__TEXT,__text,regular,pure_instructions
	.globl	_main
	.align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## BB#0:                                ## %entry
	subq	$24, %rsp
Ltmp1:
	.cfi_def_cfa_offset 32
	movl	$0, 20(%rsp)
	movl	%edi, 16(%rsp)
	movq	%rsi, 8(%rsp)
	leaq	L_.str(%rip), %rdi
	xorb	%al, %al
	callq	_printf
	xorl	%eax, %eax
	addq	$24, %rsp
	ret
	.cfi_endproc

	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	 "Hello World\n"


.subsections_via_symbols
