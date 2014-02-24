section .data

    msg     db      "A" ,0xA
    len     equ     $-msg

    msg2    db      "HELLO" ,0xA
    len2    equ     $-msg

    msg3    db      "CHECKING YOU SHIT!" ,0xA

section .text

        global _start


_start:


    mov eax ,4
    mov ebx ,1
    mov ecx ,msg3
    mov edx ,len2
    int 0x80


    mov eax ,1
    xor ebx ,ebx
    int 0x80



