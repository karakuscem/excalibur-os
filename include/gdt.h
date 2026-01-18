#ifndef GDT_H
#define GDT_H

#include <stdint.h>

struct gdt_entry_struct {
  /*
   * So the struct is messy because when intel desing 32 bit cards they don't want to break old ones so they add last bits to base_high and granularity part. This struct is basically needed for to identify spaces and accesses. limit_low was the original size limit 64kb. base_low + base_middle was original address space 16mb everything else base_high and granularity was added later to expand those numbers to 32-bit / 4gb. 
   * BAck in day we had base_low 16bit and base middle 8bit total 24 bits = 16mb of phsical ram addressable. We had 16 bits limit and total 16 bits = 64kb max size for a single chunk
   *
   * The upgrade 32 bit era, we need 32 bits for base (4gb) we had 24 bits (base_low + base middle) we added base_high(8 bits) at the end 24 + 8 = 32 bits. And for the limit we had 16bits (limit_low) we added 4 bits inside the granularity byte total 20 bits, the problem is when we want to say the size is 4gb we need 32bits of space to write that number but we only have 20bits of space you cannot fit 32 bit number into 20 bit box and granularity flag does two things flag 0 = the number in the box is in bytes and flag 1 = the number in the box is in pages (4kb blocks) and here is the trick we added a multiply by 4096 flag and 20 bits 4096 = 4gb if we fill the 20bit box with all ones unit = bytes: 1,048,575 BYTES = 1MB, Unit = Pages : 1,048,575 4096 bytes = 4GB, so we multiply by 4096 simply because thats the only way to describe the 4GB using only 20bits. 
   *
   * */
  uint16_t limit_low; // Lower 16 bits of the limit
  uint16_t base_low; // lower 16 bits of the base
  uint8_t base_middle; // the next 8 bits of the base
  uint8_t access; //access flags determine what ring this segment can be used
  uint8_t granularity; //low 4 bits are high 4 bits of limit high 4 bits are flags
  uint8_t base_high; //the last 8 bits of the base;
}__attribute__((packed));

typedef struct gdt_entry_struct gdt_entry_t;

// Also define the GDT pointer struct (used by LGDT instruction)
struct gdt_ptr_struct {
  uint16_t limit; // entire gdt table size
  uint32_t base; //the addreses of the first gdt_entry_t struct.
} __attribute__((packed));

typedef struct gdt_ptr_struct gdt_ptr_t;

// init function
void init_gdt();

#endif
