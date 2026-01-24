#include <stddef.h>
#include <stdint.h>
#include "../../include/gdt.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
volatile uint16_t* terminal_buffer;


/*
 * VGA COLORS
*/ 
enum VGA_COLORS {
  BLACK = 0x0,
  BLUE = 0x1,
  GREEN = 0x2,
  CYAN = 0x3,
  RED = 0x4,
  MAGENTA = 0x5,
  BROWN = 0x6,
  LIGHT_GREY = 0x7,
  DARK_GREY = 0x8,
  LIGHT_BLUE = 0x9,
  LIGHT_GREEN = 0xA,
  LIGHT_CYAN = 0xB,
  LIGHT_RED = 0xC,
  PINK = 0xD,
  YELLOW = 0xE,
  WHITE = 0xF
};

size_t str_len(const char* data) {
  size_t i = 0;
  while(data[i] != '\0')
    i++;
  return (i);
}

uint8_t vga_entry_color(uint8_t fg, uint8_t bg) {
  return fg | (bg << 4);
}

uint16_t vga_entry(uint8_t uc, uint8_t color) {
  return (uint16_t) uc | ((uint16_t) color << 8);
}

void terminal_initialize() {
  terminal_row = 0;
  terminal_column = 0;
  terminal_color = vga_entry_color(LIGHT_GREY, BLACK);
  terminal_buffer = (uint16_t*) 0xB8000;
  for (size_t i = 0; i < VGA_HEIGHT; i++) {
    for (size_t j = 0; j < VGA_WIDTH; j++) {
      terminal_buffer[i * VGA_WIDTH + j] = vga_entry(' ', terminal_color);
    }
  }
}

void terminal_write_string(const char* data) {
  size_t len = str_len(data);
  for (size_t i = 0; i < len; i++) {
    size_t index = terminal_row * VGA_WIDTH + terminal_column;
    terminal_buffer[index] = vga_entry(data[i], terminal_color);
    terminal_column++;
    if (terminal_column == VGA_WIDTH) {
      terminal_column = 0;
      if (terminal_row < VGA_HEIGHT) {
          terminal_row++;
      }
    }
  }
}

void kernel_main(unsigned long magic, unsigned long addr){
  init_gdt();
  terminal_initialize();
  terminal_write_string("Hello, Excalibur OS!");
}
  
