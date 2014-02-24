.MSG:
    .string     "CHECK\n"
    .text

.globl  _start

_start:

    mov     $4 ,%eax    # set syscall -- write()
    mov     $1 ,%ebx    # argument 1 of write()
    mov     $.MSG ,%ecx # argument 2 of write()
    mov     $7 ,%edx    # argument 3 of write()
    int     $0x80       # call kernel

    mov     $1 ,%eax    # set syscall -- exit()
    mov     $0 ,%ebx    # argument 1 of exit()
    int     $0x80       # call kernel
