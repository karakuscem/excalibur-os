# Phase 7: User Mode & Syscalls

This is the transition from a single program (the kernel) to an environment where other programs can run safely.

## 1. Key Terminology

### Protection Rings (Ring 0 vs. Ring 3)
*   **What it is:** Hardware-enforced security levels in the x86 CPU.
    *   **Ring 0:** Kernel Mode. Can execute any instruction (like `hlt` or `cli`).
    *   **Ring 3:** User Mode. Restricted. Cannot touch hardware directly or access kernel memory.
*   **Why we use it:** To prevent a buggy or malicious user program from crashing the whole computer.

### TSS (Task State Segment)
*   **What it is:** A structure in memory that holds information about a task. 
*   **Why we need it for User Mode:** When an interrupt occurs while in Ring 3, the CPU needs to switch to a safe Kernel Stack. The TSS tells the CPU *where* that kernel stack is located (`ss0` and `esp0`).

### Syscall (System Call)
*   **What it is:** A controlled way for a User Mode program to ask the Kernel to do something (like "write this text to the screen").
*   **Why we use it:** Since Ring 3 can't touch the VGA hardware, it must "call" the kernel to do it on its behalf.

### ELF (Executable and Linkable Format)
*   **What it is:** The standard file format for programs. It contains the code, the data, and an "Entry Point" address.
*   **Why we use it:** It's much more flexible than a simple flat binary. It allows for symbols and multiple sections.

## 2. Switching to User Mode
This is often considered one of the hardest parts of OS dev. You don't "call" user mode. Instead, you:
1.  Push the values for a User Mode environment (Stack segment, Stack pointer, EFLAGS, Code segment, Instruction pointer) onto the stack.
2.  Execute the `iret` (Interrupt Return) instruction.
3.  The CPU "returns" into the user program and drops the privilege level to Ring 3.

## 3. Implementing Syscalls
Commonly, OSes use `int 0x80`. 
1.  User program puts the syscall number in `eax` and arguments in `ebx`, `ecx`, etc.
2.  User program calls `int 0x80`.
3.  CPU switches to Ring 0 and jumps to the Syscall Handler in the kernel.
4.  Kernel performs the task and returns the result in `eax`.
