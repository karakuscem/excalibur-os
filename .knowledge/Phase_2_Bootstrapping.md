# Phase 2: Bootstrapping (Multiboot)

Bootstrapping is the process of "pulling yourself up by your bootstraps." It is the very first code that runs when the computer turns on.

## 1. Key Terminology

### Multiboot Specification
*   **What it is:** A standard for how a bootloader (like GRUB) should load an OS kernel.
*   **Why we use it:** Instead of writing our own complex bootloader that has to deal with BIOS/UEFI quirks and disk reading, we use a standard. If our kernel has a "Multiboot Header," GRUB will know how to load it.

### GRUB (Grand Unified Bootloader)
*   **What it is:** A powerful bootloader that can load many different operating systems.
*   **Why we use it:** It handles the transition from the BIOS to our kernel, provides us with information about memory, and can even load "modules" (initial files) for us.

### Linker Script (`linker.ld`)
*   **What it is:** A text file that tells the linker (`ld`) how to organize the different parts of your program (code, data, constants) in the final binary file.
*   **Why we need it:** In a normal program, the OS handles where code goes. In a kernel, we must tell the hardware exactly where the code should live in physical memory (usually starting at 1MB).

### Stack
*   **What it is:** A region of memory used for temporary data, local variables, and function call information (return addresses).
*   **Why we need it:** When the bootloader jumps to our kernel, there is no stack. If we try to call a C function without a stack, the CPU will crash immediately.

## 2. The Multiboot Header

The header must be located within the first 8192 bytes of the kernel binary and must be 32-bit aligned. It contains:
1.  **Magic Number:** `0x1BADB002` (This identifies the file as Multiboot-compliant).
2.  **Flags:** Tell the bootloader what features we need (e.g., page alignment, memory info).
3.  **Checksum:** A magic math calculation to prove the header is valid (`-(Magic + Flags)`).

## 3. The Entry Point (`boot.s`)

In assembly, we define a symbol (usually `_start`) as the global entry point.
1.  **Declaring the Header:** We use `section .multiboot` to ensure the header goes at the very beginning.
2.  **Setting up the Stack:** We reserve a small amount of memory (e.g., 16KB) and point the `esp` register to the top of it.
3.  **Calling C:** Once the stack is set, we use `extern kernel_main` and then `call kernel_main`.

## 4. The Linker Script Layout

A typical kernel linker script looks like this:
*   `.text`: The actual executable machine code.
*   `.rodata`: Read-only data (like strings).
*   `.data`: Global variables that have an initial value.
*   `.bss`: Global variables that start at zero (this saves space in the file).

**Crucial Rule:** The `.multiboot` section MUST be the first thing in the `.text` section to ensure GRUB finds it.
