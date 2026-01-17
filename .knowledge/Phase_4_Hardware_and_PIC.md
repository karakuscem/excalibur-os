# Phase 4: Hardware & PIC

To communicate with the physical hardware (keyboard, timer, disks), we need to use specialized CPU instructions.

## 1. Key Terminology

### I/O Ports
*   **What it is:** A separate address space from RAM used by the CPU to communicate with hardware controllers.
*   **Why we use it:** On x86, you don't always communicate with hardware by writing to memory. Instead, you use `in` and `out` instructions to send/receive data from "Ports" (like Port 0x60 for the keyboard).

### PIC (Programmable Interrupt Controller)
*   **What it is:** A chip (usually the Intel 8259) that manages hardware interrupts. It collects signals from devices (like the mouse) and tells the CPU to pause and handle them.
*   **Why we need to "Remap" it:** By default, the PIC sends hardware interrupts to the same IDT slots as CPU exceptions (0-15). We must tell the PIC to send them to slots 32-47 instead so we can tell the difference between a "Page Fault" and a "Timer Tick."

### IRQ (Interrupt Request)
*   **What it is:** A hardware signal sent to the PIC. For example, IRQ 1 is always the Keyboard.

### PIT (Programmable Interval Timer)
*   **What it is:** A chip that can be configured to send an interrupt at a regular interval (e.g., 100 times per second).
*   **Why we use it:** It provides the "Heartbeat" of the OS. We use it for keeping track of time and for multitasking (switching between processes).

## 2. Implementing I/O Wrappers
Since C doesn't have a built-in `inb` or `outb` function, you must write them using "Inline Assembly."
*   `outb(port, data)`: Sends a byte to a hardware port.
*   `inb(port)`: Reads a byte from a hardware port.

## 3. The PIC Remapping Sequence
This is a delicate dance of sending "Initialization Control Words" (ICWs) to the PIC ports (`0x20` and `0xA0`). 
1.  Start initialization.
2.  Set the Offset (Vector) for the Master PIC (32).
3.  Set the Offset (Vector) for the Slave PIC (40).
4.  Tell them how they are wired together.
5.  Set the environment mode (8086 mode).

## 4. The System Timer
By configuring the PIT, you can create a `sleep(ms)` function or a `get_ticks()` function, which are essential for any OS.
