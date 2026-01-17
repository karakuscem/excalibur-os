# Phase 3: Monolithic Kernel Core

Now that the CPU is executing our C code, we need to build the foundation of the kernel.

## 1. Key Terminology

### VGA Text Buffer (`0xB8000`)
*   **What it is:** A specific area of physical memory that is mapped directly to the video hardware.
*   **Why we use it:** In text mode, writing a character and a color byte to this address range will immediately display it on the screen. It is the simplest way to get visual feedback.

### GDT (Global Descriptor Table)
*   **What it is:** A table that tells the CPU about the characteristics of different memory segments (base address, size, and access privileges).
*   **Why we need it:** Even though we use a "Flat Memory Model" (where every segment covers the whole 4GB), the x86 architecture requires a GDT to be defined to handle security levels (Ring 0 for Kernel, Ring 3 for User).

### IDT (Interrupt Descriptor Table)
*   **What it is:** A table that tells the CPU which function to call when a specific "Interrupt" occurs.
*   **Why we need it:** If something goes wrong (like dividing by zero) or something happens (like a key press), the CPU needs to know which kernel function should handle it.

### ISR (Interrupt Service Routine)
*   **What it is:** A specialized function (usually starting in assembly) that handles an interrupt.
*   **Why we need it:** When an interrupt happens, the CPU stops what it's doing. The ISR must save all registers, handle the event, and then restore the registers before returning.

## 2. Implementing the VGA Driver
The VGA buffer starts at `0xB8000`. Each character on screen is represented by 2 bytes:
1.  **Character Byte:** The ASCII value.
2.  **Attribute Byte:** The foreground and background colors.

You will need to maintain a "Cursor" (X and Y coordinates) and handle the newline character (`\n`) by incrementing Y and resetting X.

## 3. Setting Up the GDT
A basic GDT for a kernel usually has 5 entries:
1.  **Null Descriptor:** Required by the CPU.
2.  **Kernel Code Segment:** Base 0, Limit 4GB, Ring 0.
3.  **Kernel Data Segment:** Base 0, Limit 4GB, Ring 0.
4.  **User Code Segment:** Base 0, Limit 4GB, Ring 3.
5.  **User Data Segment:** Base 0, Limit 4GB, Ring 3.

## 4. Setting Up the IDT
The IDT consists of 256 entries. The first 32 entries are reserved by the CPU for exceptions (Divide by zero, Page Fault, etc.). You must point these entries to your ISRs.
