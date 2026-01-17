# Phase 5: Memory Management

Memory management is the most critical part of an OS. It provides safety, isolation, and the illusion of infinite memory.

## 1. Key Terminology

### Physical Memory Manager (PMM)
*   **What it is:** The part of the kernel that keeps track of which 4KB chunks (pages) of physical RAM are currently in use and which are free.
*   **Why we need it:** When a process needs memory, the PMM finds a free spot in the actual RAM chips.

### Bitmap
*   **What it is:** A simple data structure where 1 bit represents 1 page of memory. If the bit is 1, the page is used. If 0, it is free.
*   **Why we use it:** It is very memory-efficient. To track 4GB of RAM, you only need 128KB of memory for the bitmap.

### Paging
*   **What it is:** A hardware feature that translates "Virtual Addresses" (what your program sees) into "Physical Addresses" (where the data actually lives in RAM).
*   **Why we use it:** It allows every process to think it has the whole 4GB address space to itself, without interfering with other processes.

### CR3 Register
*   **What it is:** A control register in the CPU that holds the physical address of the current "Page Directory."
*   **Why we use it:** Loading this register is what "turns on" paging for a specific process.

### Kernel Heap
*   **What it is:** A region of memory where the kernel can dynamically allocate small or variable-sized objects (like a `malloc` for the kernel).
*   **Why we use it:** You don't always know how much memory you need at compile time (e.g., for a list of open files).

## 2. PMM Implementation
You will use the "Multiboot Memory Map" (provided by GRUB) to find out where the actual RAM is and where the "holes" (reserved for hardware) are. You then mark all "available" RAM as free in your bitmap.

## 3. Paging Structure (x86)
x86 uses a two-level paging system:
1.  **Page Directory:** Contains 1024 entries. Each entry points to a Page Table.
2.  **Page Table:** Contains 1024 entries. Each entry points to a 4KB Physical Page.
Total: 1024 * 1024 * 4096 = 4GB.

## 4. Identity Mapping
When you first turn on paging, the virtual address must equal the physical address (`0x1000 == 0x1000`). If you don't do this, the CPU will immediately crash because the next instruction it tries to fetch will be at a completely different location.
