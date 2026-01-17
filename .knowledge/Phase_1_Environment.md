# Phase 1: Preparation & Environment

Setting up an environment for OS development is different from standard software development. You are targeting "Bare Metal"—meaning there is no underlying operating system to provide services like `printf`, memory management, or file I/O.

## 1. Key Terminology

### Cross-Compiler
*   **What it is:** A compiler that runs on one architecture/OS (your host, e.g., Linux on x86_64) but generates executable code for another architecture/OS (the target, e.g., i686-elf).
*   **Why we need it:** Your system's default `gcc` targets your current OS (Linux). It assumes the existence of the C standard library (libc) and specific executable formats (like Linux ELF). A cross-compiler like `i686-elf-gcc` is configured to assume *nothing* about the target environment, which is exactly what we need for a kernel.

### i686-elf-gcc
*   **What it is:** A specific version of the GNU Compiler Collection (GCC). 
    *   `i686`: Refers to the instruction set (32-bit P6 microarchitecture).
    *   `elf`: Refers to the Executable and Linkable Format, a common standard for binaries.
*   **Why we use it:** It ensures our kernel is a "freestanding" binary, free from dependencies on your host machine's libraries.

### Binutils
*   **What it is:** A collection of binary tools, including the linker (`ld`), assembler (`as`), and tools to inspect binaries (`objdump`, `nm`).
*   **Why we need it:** We need a version of these tools (`i686-elf-ld`, etc.) that understands how to link our kernel for the target architecture.

### NASM (Netwide Assembler)
*   **What it is:** An assembler for the x86 architecture. It converts human-readable assembly code into machine code.
*   **Why we use it:** We use NASM for the very early stages of booting where C code cannot run yet (e.g., setting up the stack, defining the Multiboot header).

### QEMU (Quick Emulator)
*   **What it is:** A hosted hypervisor that performs hardware virtualization. It simulates a full computer (CPU, RAM, Disk, VGA).
*   **Why we use it:** It allows us to test our OS without rebooting our physical computer every time we make a change.

## 2. Setting Up the Toolchain

### Option A: Using a Package Manager (Easiest)
On many Linux distributions, you can find pre-built cross-compilers.
*   **Ubuntu/Debian:** `sudo apt install gcc-i686-linux-gnu` (Note: sometimes you specifically want `elf` instead of `linux-gnu` to avoid Linux dependencies. If unavailable, see Option B).
*   **Arch Linux:** Available in the AUR as `i686-elf-gcc`.

### Option B: Building from Source (Recommended for Learning)
Building your own cross-compiler ensures you have exactly what you need. This involves:
1.  Downloading the source code for `binutils` and `gcc`.
2.  Configuring them with `--target=i686-elf --with-sysroot --disable-nls --disable-libssp`.
3.  Compiling them (this can take 20-60 minutes).

## 3. Verification
Once installed, verify your tools:
```bash
i686-elf-gcc --version
i686-elf-ld --version
nasm -v
qemu-system-i386 --version
```
