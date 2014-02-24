	.file	"open_write_close.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	"abc.txt"
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align 4
.LC0:
	.string	"This is an Homework that is practicing Assembly.\n"
	.text
	.p2align 4,,15
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	pushl	%eax
	andl	$-16, %esp
	subl	$24, %esp
	pushl	$1
	pushl	$.LC1
	call	open
	addl	$12, %esp
	pushl	$4
	pushl	$.LC0
	pushl	%eax
	movl	%eax, %ebx
	call	write
	movl	%ebx, (%esp)
	call	close
	xorl	%eax, %eax
	movl	-4(%ebp), %ebx
	leave
	ret
	.size	main, .-main
	.section	.note.GNU-stack,"",@progbits
	.ident	"GCC: (GNU) 3.4.4 20041218 (prerelease) (Debian 3.4.3-6)"
