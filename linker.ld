/*Bootloader jump here*/
ENTRY(_start)

SECTIONS
{
  /*Start kernel at 1MB, 0-1MB Used by bios and hardware*/
  . = 1M;
  /*Multiboot header and code part*/
  .text : ALIGN(4K) /*ALlost+found/
  {
    /*Multiboot need to be first*/
    KEEP(*(.multiboot))
    *(.text)
  }
  /*Read only data strings etc*/
  .rodata : ALIGN(4K)
  {
    *(.rodata)
  }
  /*Initialized data int a = 5;*/
  .data : ALIGN(4K)
  {
    *(.data)
  }
  /*Uninitialized data and stack Stack(int a)*/
  .bss : ALIGN(4K)
  {
    *(COMMON)
    *(.bss)
  }
  _kernel_end = .;
}
