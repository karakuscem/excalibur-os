# Phase 6: Drivers & Filesystems

A Monolithic kernel includes hardware drivers and filesystems directly in the kernel image for maximum performance.

## 1. Key Terminology

### Scancodes
*   **What it is:** The raw numbers sent by a keyboard when a key is pressed or released.
*   **Why we need it:** Pressing 'A' doesn't send the letter 'A'. It sends code `0x1E`. The OS must use a "Keymap" to translate this into ASCII.

### IDE/ATA (Integrated Drive Electronics)
*   **What it is:** An older but standard way for the CPU to talk to hard drives.
*   **Why we use it:** It is much simpler to implement than modern NVMe or SATA (AHCI) and is supported by almost all emulators and old hardware.

### PIO (Programmed I/O)
*   **What it is:** A method of transferring data where the CPU manually reads every byte from the disk controller into a register.
*   **Why we use it:** While slow compared to DMA (Direct Memory Access), it is extremely easy to write a driver for.

### VFS (Virtual File System)
*   **What it is:** An abstraction layer that defines a standard interface for all filesystems.
*   **Why we use it:** So that a function like `read_file()` works exactly the same whether the file is on a hard drive, a USB stick, or in memory.

## 2. Keyboard Driver
The keyboard sends an interrupt (IRQ 1) every time a key state changes. You read the scancode from Port `0x60`. You must handle "Shift" and "Caps Lock" by keeping track of their state in global variables.

## 3. Storage Driver (ATA PIO)
To read a sector (512 bytes) from a disk:
1.  Wait for the drive to be "Ready."
2.  Send the LBA (Logical Block Address) to the controller ports.
3.  Send the "Read" command.
4.  Loop 256 times and read 2 bytes at a time from the Data Port (`0x1F0`).

## 4. Simple Filesystem (FAT or Ext2)
A filesystem organizes raw disk sectors into "Files" and "Directories." You will need to implement:
*   `find_file(path)`
*   `read_directory(path)`
*   `read_file_data(inode/cluster)`
