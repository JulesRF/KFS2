bits 32

section .multiboot
        dd 0x1BADB002               ; Magic number for bootloader
        dd 0x0                      ; Flags
        dd - (0x1BADB002 + 0x0)     ; Checksum

section .text
global start
extern main                         ; Main function (from C code)
extern gp                           ; GDT pointer declared in C

global load_gdt     ; Export the function for use in C

load_gdt:
    mov eax, [esp+4]  ; Load the GDT pointer (argument from C)
    lgdt [eax]        ; Load the new GDT
    ret               ; Return to the caller (C function)

start:
    cli                             ; Disable interrupts

    lgdt [gp]                       ; Load the Global Descriptor Table

    sti                             ; Re-enable interrupts
    xor ebp, ebp
    mov esp, stack_space            ; Set up stack

    push ebx
    call main                       ; Call kernel main function

    jmp .halt                       ; Halt CPU
.halt:
    hlt
    jmp .halt

section .bss
    resb 4096                       ; Stack size: 4KB
stack_space:
