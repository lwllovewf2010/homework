	.file	"write_stdout.c"
	.section	.rodata.str1.1,"aMS",@progbits,1



.LC0:
	.string	"CHECK\n"
	.text
	.p2align 4,,15



    
.globl main
	.type	main, @function
main:



	pushl	%ebp        ; 將 ebp 存入堆疊 ,Base Pointer(基底指標)
	movl	%esp, %ebp  ; 將 esp(Stack Pointer) 放入 ebp
	subl	$8, %esp    ; esp - 8 (0000 1000)
	andl	$-16, %esp  ; esp AND -16
	subl	$20, %esp   ; esp - 20

	pushl	$7          ; argument 3 -- 7 個 ASCII code (length)
	pushl	$.LC0       ; argument 2 -- 放入字串 label (位址)
	pushl	$1          ; argument 1 -- stdout 的編號 = 1
	call	write       ; call kernel


	xorl	%eax, %eax
	leave
	ret
	.size	main, .-main
	.section	.note.GNU-stack,"",@progbits
	.ident	"GCC: (GNU) 3.4.4 20041218 (prerelease) (Debian 3.4.3-6)"
