BITS 32

section .multiboot
    dd 0x1BADB002            ; magic
    dd 0x00000003            ; flags: page align + mem info
    dd -(0x1BADB002 + 0x00000003)

section .bss
stack_bottom:
    resb 16384               ; 16 KiB kernel stack
stack_top:

section .text
global _start
extern kmain

_start:
    cli
    mov esp, stack_top
    push ebx                 ; multiboot info pointer
    push eax                 ; multiboot magic
    call kmain
.hang:
    hlt
    jmp .hang
