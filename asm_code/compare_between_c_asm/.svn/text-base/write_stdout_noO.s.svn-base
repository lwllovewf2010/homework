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



	subl	$4, %esp        ; use syscall -- write
	pushl	$7              ; argument 3
	pushl	$.LC0           ; argument 2
	pushl	$1              ; argument 1
	call	write           ; call kernel



	addl	$16, %esp
	movl	$0, %eax


	leave
	ret
	.size	main, .-main
	.section	.note.GNU-stack,"",@progbits
	.ident	"GCC: (GNU) 3.4.4 20041218 (prerelease) (Debian 3.4.3-6)"
