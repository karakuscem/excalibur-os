; ==============================================================================
; BOOT.S - Kernel Entry Point & Multiboot Header
; ==============================================================================
; This file defines the entry point for the kernel and the Multiboot header
; required by the bootloader (GRUB). It initializes the stack and transitions
; control to the C kernel.

; ------------------------------------------------------------------------------
; Multiboot Header Constants
; ------------------------------------------------------------------------------
; Defined in the Multiboot Specification. These flags tell the bootloader
; how to load our kernel and what information to provide.
; ------------------------------------------------------------------------------
MBALIGN  equ  1 << 0            ; Align loaded modules on 4KB page boundaries
MEMINFO  equ  1 << 1            ; Request memory map from bootloader
FLAGS    equ  MBALIGN | MEMINFO ; Combined flags
MAGIC    equ  0x1BADB002        ; Magic number to identify as Multiboot-compliant
CHECKSUM equ -(MAGIC + FLAGS)   ; Checksum verification: (Magic + Flags + Checksum) == 0

; ------------------------------------------------------------------------------
; Multiboot Section
; ------------------------------------------------------------------------------
; The linker script places this section at the very beginning of the kernel binary.
; The bootloader searches the first 8KB of the file for the Magic number.
; ------------------------------------------------------------------------------
section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

; ------------------------------------------------------------------------------
; Stack Allocation
; ------------------------------------------------------------------------------
; The C calling convention (System V ABI) requires a valid stack.
; We reserve 16KB of uninitialized memory in the BSS section.
; On x86, the stack grows downwards (from high to low memory).
; ------------------------------------------------------------------------------
section .bss
align 16
stack_bottom:
    resb 16384 ; Reserve 16KB
stack_top:

; ------------------------------------------------------------------------------
; Kernel Entry Point
; ------------------------------------------------------------------------------
; The linker script specifies '_start' as the entry symbol.
; ------------------------------------------------------------------------------
section .text
global _start
extern kernel_main

_start:
    ; 1. Initialize the Stack Pointer
    ;    Point ESP to the top of the allocated stack memory.
    mov esp, stack_top

    ; 2. Push Multiboot Parameters
    ;    GRUB passes specific values in registers:
    ;    EBX: Pointer to Multiboot Information Structure
    ;    EAX: Multiboot Magic Number (0x2BADB002)
    ;    We push these to pass them as arguments to 'kernel_main'.
    push ebx
    push eax

    ; 3. Transfer Control to C
    ;    Call the main kernel function.
    call kernel_main

    ; 4. Hang (Safety Fallback)
    ;    If kernel_main returns, disable interrupts and halt the CPU
    ;    in an infinite loop to prevent executing garbage memory.
    cli
.hang:
    hlt
    jmp .hang   ; If it wakes up, force it back to sleep (Infinite Loop)

