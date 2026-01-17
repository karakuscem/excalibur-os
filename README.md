# Excalibur OS

## Project Purpose
Excalibur OS is an educational project dedicated to building an operating system from scratch ("Bare Metal") on the x86 architecture. 

**Core Philosophy:**
The primary goal is **deep learning**, not just functional output. We are building this with **AI guidance**, where the AI acts as a mentor and reference, not a code generator. This is **strictly not "vibe coding"**; every line of code, every linker command, and every assembly instruction is researched, understood, and implemented manually to ensure full comprehension of the underlying systems.

## Devlog: Saturday, January 17, 2026

**Session 1: Environment & Toolchain**
We successfully established the development environment required for OS development, moving away from the host's standard toolchain to a cross-compilation environment.

1.  **Cross-Compiler Setup:**
    *   Validated the absence of a pre-existing cross-compiler.
    *   Located the partial build of GCC.
    *   Completed the compilation of `i686-elf-gcc` and `libgcc` from source.
    *   Installed the toolchain to `~/opt/cross`.
    *   **Verification:** Confirmed `i686-elf-gcc` version 13.2.0 is functional.

2.  **Project Initialization:**
    *   Created the standard directory structure:
        *   `src/arch/i386` (Architecture-specific assembly).
        *   `src/kernel` (Kernel C code).
        *   `include` (Header files).
    *   Initialized the core bootstrapping files: `boot.s`, `kernel.c`, and `linker.ld`.

3.  **Bootstrapping - Phase 1 (Linker):**
    *   Researched the role of the Linker Script (`.ld`).
    *   **Implemented `linker.ld`:**
        *   Defined the entry point (`ENTRY(_start)`).
        *   Set the kernel load address to **1MB** (`0x100000`) to avoid BIOS/VGA conflicts.
        *   Ordered sections to ensure the **Multiboot Header** appears first in the binary (critical for GRUB detection).
        *   Aligned sections to 4KB (page boundaries) for future memory management compatibility.
        *   Handled the `.bss` and `*(COMMON)` sections for uninitialized data.

---

## Current Status & To-Do

### Phase 2: Bootstrapping
- [x] **Setup Cross-Compiler (`i686-elf-gcc`)**
- [x] **Create Project Structure**
- [x] **Implement Linker Script (`linker.ld`)**
- [ ] **Implement Assembly Bootstrap (`src/arch/i386/boot.s`)**
    - [ ] Define Multiboot Header (Magic `0x1BADB002`, Flags, Checksum).
    - [ ] Allocate Kernel Stack (16KB).
    - [ ] Implement `_start` entry point.
    - [ ] Disable interrupts (`cli`) and set up Stack Pointer (`esp`).
    - [ ] Call `kernel_main`.
    - [ ] Implement infinite halt loop.
- [ ] **Implement Minimal Kernel (`src/kernel/kernel.c`)**
    - [ ] Create VGA driver pointers (`0xB8000`).
    - [ ] Write "Hello World" or verification characters to video memory.
- [ ] **Build & Run**
    - [ ] Assemble `boot.s` with NASM.
    - [ ] Compile `kernel.c` with GCC.
    - [ ] Link using the Linker Script.
    - [ ] Verify Multiboot compliance (`grub-file --is-x86-multiboot`).
    - [ ] Boot in QEMU.
