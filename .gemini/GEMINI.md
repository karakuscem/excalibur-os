# Gemini Agent Context & Instructions

This file serves as the persistent brain for the AI agent. It contains the project's technical DNA, operational protocols, and learned user preferences. **Read this at the start of every session.**

## 1. Project Identity
*   **Name:** Excalibur OS
*   **Type:** 32-bit x86 Monolithic Kernel.
*   **Repo Root:** `/home/ckarakus/Desktop/github/excalibur-os`
*   **Philosophy:** **"Deep Understanding."**
    *   We do not use high-level abstractions without building them first.
    *   We do not copy-paste code without explaining the *why*.
    *   We prioritize "Bare Metal" implementation details over rapid feature creep.
    *   **Anti-Pattern:** "Vibe Coding" (mindless generation) is strictly forbidden.

## 2. Technical Architecture (The "Hard" Rules)
*   **Target Arch:** `i686-elf` (Intel 32-bit Protected Mode).
*   **Boot Protocol:** Multiboot 1 Specification.
*   **Toolchain:**
    *   **Compiler:** `i686-elf-gcc` (Freestanding, no libc).
    *   **Assembler:** `nasm` (Intel Syntax). **NEVER use GAS (GNU Assembler) syntax.**
    *   **Linker:** `i686-elf-ld`.
    *   **Emulator:** `qemu-system-i386`.
*   **Memory Map:**
    *   `0x00000000` - `0x00100000`: Reserved (BIOS, VGA, IO).
    *   `0x00100000` (1MB): **Kernel Entry Point**.
    *   `0xC0000000` (3GB): Higher Half Kernel mapping (Planned for Phase 5).
*   **Conventions:**
    *   **Stack Alignment:** 16 Bytes (Strict System V ABI compliance).
    *   **Comments:** Professional, architectural descriptions. Avoid analogies in code.

## 3. Operational Workflows

### A. The "Implement" Workflow
When asked to implement a feature (e.g., "Add GDT"):
1.  **Research:** Briefly verify the hardware specification (e.g., "GDT requires a null descriptor...").
2.  **Plan:** Explain the specific bits and bytes to the user.
3.  **Code:** Write the file using established conventions.
4.  **Verify:** Check for compilation errors immediately.

### B. The "Explain" Workflow
When asked to explain a concept:
1.  **Analogy Phase:** Use relatable concepts (e.g., "The Linker is a Librarian") *if* the user asks for a "WebDev" or "Beginner" explanation.
2.  **Technical Phase:** Drill down into registers, memory addresses, and CPU flags.
3.  **Documentation (STRICT MANDATE):**
    *   **Trigger:** Any time a technical term, architectural concept, or keyword (e.g., GDT, Paging, ISR) is explained or introduced.
    *   **Action:** Immediately update `LEARNED.md` with a detailed, professional technical entry.
    *   **Verification:** Before ending any session, verify that all key concepts discussed during that session are accurately reflected in `LEARNED.md`.
    *   **Content:** Avoid analogies in the documentation; focus on hardware specifications, bit layouts, and system-level logic.

### C. The "Maintenance" Workflow
After *every* significant code change:
1.  **Update README.md:** Check off the specific task in the To-Do list.
2.  **Update LEARNED.md:** Log new technical facts.
3.  **Check Consistency:** Ensure `boot.s`, `linker.ld`, and `kernel.c` agree on addresses and names.

## 4. Persistent User Facts
*   **Environment:** Linux (Ubuntu-like).
*   **Cross-Compiler Path:** `~/opt/cross/bin` (Must be in PATH).
*   **Preferred Tone:** Professional, mentoring, authoritative but helpful.
*   **Code Style:**
    *   **ASM:** NASM, heavily commented with architectural details.
    *   **C:** Standard GNU style, clear variable names, no "magic numbers" without `#define`.

## 5. Detailed Roadmap & To-Do List

### Phase 2: Bootstrapping (Current)
- [x] Build `i686-elf` Cross-Compiler.
- [x] Define `linker.ld` with 1MB load address and 4KB alignment.
- [x] Implement `boot.s` with Multiboot Header and Stack initialization.
- [x] **Implement `kernel.c`**
    - [x] Define `kernel_main` with Multiboot arguments.
    - [x] Create `terminal_initialize` to clear the VGA buffer (`0xB8000`).
    - [x] Implement `terminal_putchar` and `terminal_writestring`.
    - [x] Verify boot in QEMU.

### Phase 3: Kernel Core (Next)
- [ ] **Global Descriptor Table (GDT)**
    - [ ] Define GDT entry structure (`limit`, `base`, `access`, `flags`).
    - [ ] Create GDT pointer structure.
    - [ ] Implement `gdt_flush` in assembly to load GDTR and reload segment registers.
- [ ] **Interrupt Descriptor Table (IDT)**
    - [ ] Define IDT entry structure.
    - [ ] Create IDT pointer structure.
    - [ ] Implement `idt_load` assembly wrapper.
- [ ] **Interrupt Service Routines (ISRs)**
    - [ ] Map CPU Exceptions (0-31) to C handlers.
    - [ ] Implement common ISR assembly stub (save/restore registers).

### Phase 4: Hardware & PIC
- [ ] **Programmable Interrupt Controller (PIC)**
    - [ ] Remap PIC offsets to avoid conflicts with exceptions (0x20 - 0x28).
    - [ ] Implement IRQ handlers (Keyboard, Timer).
- [ ] **Programmable Interval Timer (PIT)**
    - [ ] Configure PIT frequency (e.g., 100Hz).
    - [ ] Implement a `sleep()` function using timer ticks.

### Phase 5: Memory Management
- [ ] **Physical Memory Manager (PMM)**
    - [ ] Parse Multiboot memory map.
    - [ ] Implement a Bitmap or Stack-based allocator for 4KB frames.
- [ ] **Virtual Memory Manager (VMM/Paging)**
    - [ ] Create Page Directory and Page Tables.
    - [ ] Implement Identity Mapping for the first 1MB.
    - [ ] Implement "Higher Half" Kernel mapping (map `0xC0000000` to `0x00100000`).
    - [ ] Enable Paging (Set CR0 bit 31).

### Phase 6: Drivers & Filesystem
- [ ] **Keyboard Driver:** Scancode to ASCII mapping.
- [ ] **ATA/IDE Driver:** Read/Write sectors from disk.
- [ ] **VFS/Filesystem:** Implement a basic InitRD or FAT12 filesystem.

### Phase 7: User Mode & Syscalls
- [ ] **TSS (Task State Segment):** Required for ring 3 transitions.
- [ ] **User Mode Switch:** Jump to Ring 3.
- [ ] **System Calls:** Implement `int 0x80` or `sysenter` handler.

### Phase 8: Multitasking
- [ ] **Process/Thread Structure:** Define PCB (Process Control Block).
- [ ] **Context Switching:** Save/Restore registers on timer interrupt.
- [ ] **Scheduler:** Implement Round Robin or Priority-based scheduling.# AI Assistance Rules for Excalibur OS

To ensure the developer learns and maintains full control over the architecture of Excalibur OS, the following rules apply to all AI interactions:

1. **Guide, Don't Dictate:** Do not provide full code implementations unless explicitly requested for boilerplate that is standard and non-creative.
2. **Socratic Method:** When a problem is presented, ask leading questions or point to the relevant concept (e.g., "Have you considered how the stack is aligned before calling C code?") rather than providing the fix immediately.
3. **Conceptual Explanations:** Focus on explaining the "why" behind OS concepts (e.g., why we need a GDT, how paging works) instead of just the "how".
4. **Documentation First:** Direct the developer to relevant documentation (Intel Manuals, OSDev Wiki) before providing an answer.
5. **Incremental Progress:** Break down tasks into the smallest possible logical units.
6. **Code Reviews:** When code is provided by the developer, critique it for safety, efficiency, and OS-specific pitfalls (e.g., race conditions, stack overflows) without rewriting the whole thing.
7. **Terminology Explanation:** Every time a specialized term (e.g., "Cross-Compiler"), tool (e.g., "i686-elf-gcc"), or low-level concept is introduced, provide a brief, clear explanation of what it is and why it is used in the context of OS development.
