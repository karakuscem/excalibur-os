# What I Have Learned
A summary of technical concepts encountered during the Excalibur OS development.

## 1. Toolchain & Environment
*   **Cross-Compiler (`i686-elf-gcc`):** A standard compiler targets the host OS (Linux). An OS kernel requires a "freestanding" compiler that makes no assumptions about existing libraries (libc) or system calls.
*   **Binutils:** We use `ld` (Linker) and `as` (Assembler) specifically targeted for the `elf` (Executable and Linkable Format) output on `i686` (32-bit x86) architecture.

## 2. The Boot Process
*   **Multiboot Specification:** A standard protocol that allows a bootloader (GRUB) to load any compliant kernel.
    *   **Magic Number (`0x1BADB002`):** A hardcoded signature in the first 8KB of the binary that identifies the file as a valid kernel.
    *   **Checksum:** A validation mechanism `-(Magic + Flags)` ensuring header integrity.
*   **Handover State:**
    *   The CPU is in **Protected Mode** (32-bit).
    *   **Interrupts** are disabled.
    *   **Paging** is disabled (Linear memory model).
    *   **Registers:** `EAX` holds a confirmation magic number (`0x2BADB002`), and `EBX` holds a pointer to the Multiboot Information Structure.

## 3. Linker Script (`linker.ld`)
*   **Physical Address Load:** We explicitly tell the linker to load our code at **1MB** (`0x100000`).
    *   *Reason:* Lower memory is fragmented by BIOS reserved areas, Video RAM (`0xA0000`), and memory-mapped hardware. 1MB is the first safe contiguous block.
*   **Section Ordering:**
    *   `.multiboot`: Must be first (for GRUB detection).
    *   `.text`: Executable code.
    *   `.rodata`: Read-only constants.
    *   `.data`: Initialized variables.
    *   `.bss`: Uninitialized memory (zeroed out).
*   **Alignment:** Sections are aligned to **4KB (`0x1000`)**.
    *   *Reason:* x86 Paging works in 4KB chunks. Aligning sections now simplifies memory protection and mapping later.

## 4. x86 Assembly (`boot.s`)
*   **The Stack:** C functions require a stack frame. The CPU does not provide one at boot.
    *   We manually reserve memory (`resb 16384`) in `.bss`.
    *   We initialize the **Stack Pointer (`ESP`)** to the *top* of this memory block because x86 stacks grow downwards.
*   **Calling Convention:**
    *   Arguments are passed by **pushing** them onto the stack in reverse order.
    *   `push ebx` (Pointer) -> `push eax` (Magic) -> `call kernel_main`.
*   **Safety Net:** An infinite loop (`cli`, `hlt`, `jmp`) is required at the end of `_start` to prevent the CPU from executing random memory if the kernel returns.
