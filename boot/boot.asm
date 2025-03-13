bits 32

section .multiboot
        dd 0x1BADB002               ; Multiboot magic number
        dd 0x0                      ; Flags (No video mode)
        dd - (0x1BADB002 + 0x0)     ; Checksum

section .text
global start
extern main                         ; Main function (from C)
extern gdt_ptr                      ; GDT pointer declared in C

global load_gdt                     ; Export the function for C

load_gdt:
    mov eax, [esp+4]                ; Load the GDT pointer (argument from C)
    lgdt [eax]                      ; Load the new GDT

    ; Perform a far jump to reload CS (important for GDT to take effect)
    jmp 0x08:.flush                 ; 0x08 = Code Segment Selector

.flush:
    mov ax, 0x10                    ; Load Data Segment Selector (0x10)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret

start:
    cli                             ; Disable interrupts

    ; Load the GDT using the function in C (safer than directly using lgdt here)
    push gdt_ptr
    call load_gdt

    sti                             ; Re-enable interrupts

    ; Initialize stack
    mov esp, stack_space + 4096
    xor ebp, ebp

    ; Call main function from C
    push ebx                        ; Multiboot info structure (GRUB passes this)
    call main

    ; Halt CPU if main returns
    jmp .halt
.halt:
    hlt
    jmp .halt

section .bss
    resb 4096                       ; Reserve 4KB for stack
stack_space:
