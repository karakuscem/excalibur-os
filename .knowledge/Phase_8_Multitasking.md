# Phase 8: Multitasking

Multitasking allows the OS to run multiple programs "simultaneously" by switching between them very quickly.

## 1. Key Terminology

### Context Switching
*   **What it is:** The process of saving the current state of the CPU (registers, stack pointer, program counter) for one process and loading the saved state of another process.
*   **Why we need it:** So that when a process resumes, it has no idea it was ever paused.

### PCB (Process Control Block)
*   **What it is:** A data structure (a C `struct`) in the kernel that stores everything about a process: its ID, its priority, its saved registers, and its page directory.
*   **Why we use it:** To keep track of all the different programs currently "living" in the OS.

### Scheduler
*   **What it is:** The part of the kernel that decides *which* process should run next.
*   **Why we use it:** To ensure fairness and efficiency.

### Round Robin
*   **What it is:** The simplest scheduling algorithm. Every process gets a fixed amount of time (a "quantum"), and then the scheduler moves to the next one in a circle.
*   **Why we use it:** It is easy to implement and prevents any single process from hogging the CPU forever.

## 2. The Context Switch Mechanism
A context switch usually happens inside an interrupt handler (like the Timer interrupt).
1.  **Save:** Push all registers onto the current process's kernel stack.
2.  **Store:** Save the current stack pointer (`esp`) into the current PCB.
3.  **Select:** Ask the scheduler for the next PCB.
4.  **Switch Memory:** If the next process has a different page directory, load it into the `CR3` register.
5.  **Restore:** Load the stack pointer (`esp`) from the next PCB.
6.  **Pop:** Pop all the saved registers from the new stack and execute `iret`.

## 3. Creating a New Process
To "spawn" a process, you must manually create a stack that *looks* like a context switch just happened. You push the entry point of the program as the "Return Address" so that when the scheduler first switches to it, the `iret` instruction jumps straight to the start of the program.
