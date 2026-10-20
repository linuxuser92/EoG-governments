BITS 32
section .text

extern isr_handler
extern timer_handler

global isr0
isr0:
    cli
    pusha
    call isr_handler
    popa
    iret

global isr32
isr32:
    cli
    pusha
    call timer_handler
    popa
    iret