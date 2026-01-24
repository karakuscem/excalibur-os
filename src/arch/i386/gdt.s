; ==============================================================================
; GDT_FLUSH.S - GDT Loading Logic
; ==============================================================================
; This assembly stub performs the privileged 'lgdt' instruction and reloads
; the segment registers to finalize the transition to our new GDT.
; ==============================================================================

global gdt_flush

gdt_flush:
    ; The pointer to the gdt_ptr struct is passed as the first argument
    ; on the stack at [esp + 4]. We move it into EAX.
    mov eax, [esp + 4]

    ; Load the new GDT pointer into the CPU's GDTR register.
    lgdt [eax]

    ; Reload the data segment registers.
    ; 0x10 (16) is the offset to the second entry in our GDT (Kernel Data).
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Perform a far jump to reload the CS (Code Segment) register.
    ; 0x08 (8) is the offset to the first entry in our GDT (Kernel Code).
    ; This forces the CPU to use the new code segment descriptor.
    jmp 0x08:.flush

.flush:
    ret
