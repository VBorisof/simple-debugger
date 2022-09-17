section .data
	msg1 db    'Hello, World!', 0xA
	len1 equ    $ - msg1
	msg2 db    'And Hello Again!', 0xA
	len2 equ    $ - msg2

section .text
    ; The _start symbol must be declared for the linker (ld)
    global     _start

_start:
    ; Prepare arguments for the sys_write system call:
    ;   - eax: system call number (sys_write)
    ;   - ebx: file descriptor (stdout)
    ;   - ecx: pointer to string
    ;   - edx: string length
    ; Print first message
    mov    eax, 4
    mov    ebx, 1
    mov    edx, len1
    mov    ecx, msg1
    ; Execute the sys_write system call
    int    0x80

    ; Print second message
    mov    eax, 4
    mov    ebx, 1
    mov    edx, len2
    mov    ecx, msg2
    ; Execute the sys_write system call
    int    0x80

    ; Execute sys_exit
    mov    eax, 1
    int    0x80

