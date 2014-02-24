	.file	"write_stdout.c"
	.section	.rodata
.LC0:
	.string	"CHECK\n"
	.text
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	andl	$-16, %esp
	movl	$0, %eax
	addl	$15, %eax
	addl	$15, %eax
	shrl	$4, %eax
	sall	$4, %eax
	subl	%eax, %esp
	subl	$4, %esp
	pushl	$7
	pushl	$.LC0
	pushl	$1
	call	write
	addl	$16, %esp
	movl	$0, %eax
	leave
	ret
	.size	main, .-main
	.section	.note.GNU-stack,"",@progbits
	.ident	"GCC: (GNU) 3.4.4 20041218 (prerelease) (Debian 3.4.3-6)"
