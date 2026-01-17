# Excalibur OS Development Guide (x86 Monolithic)

This guide is tailored for a **32-bit x86 (i686)** kernel using the **Multiboot** protocol with a **Monolithic** architecture.

## Phase 1: Preparation & Environment
*   **Target:** `i686-elf`
*   **Cross-Compiler:** You need `i686-elf-gcc` and `i686-elf-binutils`. Standard system GCC will produce binaries that won't run on bare metal.
*   **Emulator:** QEMU (`qemu-system-i386`).
*   **Tools:** `nasm` (assembly), `make`, `grub-mkrescue`, `xorriso`.

## Phase 2: Bootstrapping (Multiboot)
The bootloader (GRUB) will look for a specific magic header in your kernel.

1.  **Multiboot Header:** Create `boot.s` using NASM. Define the Multiboot header (Magic, Flags, Checksum).
2.  **Kernel Entry:** The `_start` symbol where the bootloader jumps. Here, you must:
    *   Set up a stack (the CPU doesn't give you one).
    *   Call your C `kernel_main`.
3.  **Linker Script:** `linker.ld` must place the Multiboot header within the first 8KB of the file and set the load address to 1MB (`0x100000`).

## Phase 3: Monolithic Kernel Core
In a monolithic design, your core drivers start here.

1.  **VGA Terminal Driver:** Write to `0xB8000`. Manage rows, columns, and scrolling.
2.  **Global Descriptor Table (GDT):** Even though Multiboot sets up a basic GDT, you must create your own to define code and data segments for the kernel.
3.  **Interrupts (IDT):** 
    *   Handle CPU exceptions (Faults, Traps).
    *   Implement `ISR` (Interrupt Service Routines) in assembly to save registers.

## Phase 4: Hardware & PIC
1.  **I/O Ports:** Implement `inb` and `outb` wrapper functions using inline assembly.
2.  **Programmable Interrupt Controller (PIC):** Remap the PIC so hardware IRQs (keyboard, timer) start at interrupt vector 32, avoiding conflicts with CPU exceptions.
3.  **The Timer (PIT):** Set up the 8253 PIT for system ticks.

## Phase 5: Memory Management (Monolithic Approach)
1.  **Physical Memory Manager (PMM):** A bitmap is recommended for x86 to track 4KB blocks of the 4GB address space.
2.  **Paging:** Initialize the Directory and Page Tables. Identity map the first 4MB to start, then move to a higher-half kernel if desired.
3.  **Kernel Heap:** A simple linked-list allocator to support `kmalloc`.

## Phase 6: Drivers & Filesystem
Since we are Monolithic, these will be compiled into the kernel:
1.  **Keyboard Driver:** Map scancodes to ASCII.
2.  **IDE/ATA Driver:** Read sectors from the hard disk using PIO mode.
3.  **Ext2 or FAT32:** Implement a basic filesystem driver.
4.  **VFS Layer:** Create a standard interface for file operations (`open`, `read`, `write`).

## Phase 7: User Mode & Syscalls
1.  **Task State Segment (TSS):** Setup is required to switch stacks during an interrupt from User Mode to Kernel Mode.
2.  **User Segment:** Create GDT entries for User Code and User Data (Ring 3).
3.  **Syscall Dispatcher:** Use `int 0x80` as the gateway for user programs to request kernel services.

## Phase 8: Multitasking
1.  **Process Control Block (PCB):** Store registers, stack pointer, and page directory for each process.
2.  **Scheduler:** A simple Round Robin scheduler triggered by the PIT.