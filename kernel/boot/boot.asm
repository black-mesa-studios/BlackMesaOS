
; Multiboot2 header
MAGIC    equ 0xE85250D6
ARCH     equ 0
LENGTH   equ multiboot_end - multiboot_start
CHECKSUM equ -(MAGIC + ARCH + LENGTH)

section .multiboot
align 8

multiboot_start:
    dd MAGIC
    dd ARCH
    dd LENGTH
    dd CHECKSUM

    dw 0
    dw 0
    dd 8
multiboot_end:


; Uninitialized data
section .bss
align 16

stack_bottom:
    resb 16384
stack_top:

align 4096
p4_table:
    resb 4096
p3_table:
    resb 4096
p2_table:
    resb 4096


; Global Descriptor Table
section .data
align 8

gdt64:
    dq 0

CODE_SEG equ $ - gdt64

    dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53)

gdt64_ptr:
    dw $ - gdt64 - 1
    dq gdt64


; Kernel entry point (32-bit protected mode)
section .text
BITS 32

global _start
extern kmain

_start:
    cli
    mov esp, stack_top

    ; Set up P4 -> P3
    mov eax, p3_table
    or eax, 0b11
    mov dword [p4_table], eax

    ; Set up P3 -> P2
    mov eax, p2_table
    or eax, 0b11
    mov dword [p3_table], eax

    ; Map the first 2 MiB using a huge page
    mov eax, 0x00000000
    or eax, 0b10000011
    mov dword [p2_table], eax

    ; Load P4 table into CR3
    mov eax, p4_table
    mov cr3, eax

    ; Enable PAE
    mov eax, cr4
    or eax, (1 << 5)
    mov cr4, eax

    ; Enable long mode in EFER
    mov ecx, 0xC0000080
    rdmsr
    or eax, (1 << 8)
    wrmsr

    ; Enable paging
    mov eax, cr0
    or eax, (1 << 31)
    mov cr0, eax

    ; Load 64-bit GDT
    lgdt [gdt64_ptr]

    ; Far return into 64-bit code segment
    push dword CODE_SEG
    push dword _start64
    retf


; 64-bit long mode
BITS 64

_start64:
    xor eax, eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Enter the kernel
    call kmain

.hang:
    cli
    hlt
    jmp .hang
