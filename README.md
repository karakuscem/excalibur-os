# Excalibur OS

## Project Purpose
Excalibur OS is an educational project dedicated to building an operating system from scratch ("Bare Metal") on the x86 architecture. 

**Core Philosophy:**
The primary goal is **deep learning**, not just functional output. We are building this with **AI guidance**, where the AI acts as a mentor and reference, not a code generator. This is **strictly not "vibe coding"**; every line of code, every linker command, and every assembly instruction is researched, understood, and implemented manually to ensure full comprehension of the underlying systems.

## Devlog: Sunday, January 18, 2026

**Session 2: Phase 3 - Kernel Core (GDT)**
We transitioned from a bootable prototype to building the kernel's foundational security and memory structures.

1.  **GDT Research & Design:**
    *   Deep-dived into the x86 Segment Descriptor structure.
    *   Discussed the historical evolution from 16-bit to 32-bit (base/limit fragmentation).
    *   Understood the **Granularity Flag** and the 4KB multiplier trick to address 4GB of RAM with 20 bits.
    *   **Implemented `include/gdt.h`:** Defined the packed structures for `gdt_entry_t` and `gdt_ptr_t`.

2.  **Infrastructure Updates:**
    *   Updated `LEARNED.md` with detailed GDT technical specifications.
    *   Updated `.gemini/GEMINI.md` with a granular roadmap and refined AI assistance rules.

---

## Current Status & To-Do

### Phase 2: Bootstrapping
- [x] **Setup Cross-Compiler (`i686-elf-gcc`)**
- [x] **Create Project Structure**
- [x] **Implement Linker Script (`linker.ld`)**
- [x] **Implement Assembly Bootstrap (`src/arch/i386/boot.s`)**
- [x] **Implement Minimal Kernel (`src/kernel/kernel.c`)**
- [x] **Build & Run** (Confirmed in QEMU)

### Phase 3: Kernel Core
- [ ] **Global Descriptor Table (GDT)**
    - [x] Define GDT structures in `include/gdt.h`.
    - [ ] Implement `gdt_set_gate` and `init_gdt` in `src/kernel/gdt.c`.
    - [ ] Implement `gdt_flush` in `src/arch/i386/gdt.s`.
- [ ] **Interrupt Descriptor Table (IDT)**
- [ ] **Interrupt Service Routines (ISRs)**

## How to Build & Run
To compile and run the kernel, use the following commands:
```bash
# Assemble and Compile
mkdir -p build
nasm -felf32 src/arch/i386/boot.s -o build/boot.o
i686-elf-gcc -c src/kernel/kernel.c -o build/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

# Link
i686-elf-gcc -T linker.ld -o build/myos.bin -ffreestanding -O2 -nostdlib build/boot.o build/kernel.o -lgcc

# Run
qemu-system-i386 -kernel build/myos.bin
```
