MESSAGE:
    .string     "This is an Homework that is practicing Assembly.\n"
    MSGLEN = . - MESSAGE

.FILENAME:
    .string     "abc.txt"

    .text

.globl  _start
_start:

    #################################################################
    # do open (Return Value will be in %eax)
    #################################################################
    mov     $5 ,%eax            # use syscall 5 -- open
    mov     $.FILENAME ,%ebx    # Argument 1 -- FileName
    mov     $1 ,%ecx            # Argument 2 -- Option Mask(O_WRONLY)
    int     $0x80               # call kernel


    #################################################################
    # do write
    #################################################################
    mov     %eax ,%edi          # Store "File Descriptor" in %edi
    mov     $4 ,%eax            # use syscall 4 -- write
    mov     %edi ,%ebx          # Argument 1 -- File Descriptor
    mov     $MESSAGE ,%ecx      # Argument 2 -- Message Body
    mov     $MSGLEN ,%edx       # Argument 3 -- The length of Message
    int     $0x80               # call kernel

    #################################################################
    # do close
    #################################################################
    mov     $6 ,%eax            # use syscall 6 -- close
    mov     %edi ,%ebx          # Argument 1 -- File Descriptor
    int     $0x80               # call kernel

    #################################################################
    # do exit(0)
    #################################################################
    mov     $1 ,%eax            # use syscall 1 -- exit
    mov     $0 ,%ebx            # Argument 1 -- value : 1
    int     $0x80               # call kernel


