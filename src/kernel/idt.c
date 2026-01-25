#include "../../include/idt.h"
#include "../../include/excalibur_str.h"

idt_entry_t idt_entries[256];
idt_ptr_t idt_ptr;
extern void idt_flush(uint32_t);


void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
  idt_entries[num].base_low = base & 0xFFFF;
  idt_entries[num].base_high = (base >> 16) & 0xFFFF;
  idt_entries[num].selector = sel;
  idt_entries[num].zero = 0;
  idt_entries[num].attributes = flags;
}

void init_idt(){
  idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
  idt_ptr.base = (uint32_t)&idt_entries;

  memset(&idt_entries, 0, sizeof(idt_entry_t) * 256);

  idt_flush((uint32_t)&idt_ptr);
}

