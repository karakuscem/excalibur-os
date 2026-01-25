#ifndef EXCALIBUR_STR_H
#define EXCALIBUR_STR_H

#include <stddef.h>

/**
 * memset - Fill memory with a constant byte
 * @dest: Pointer to the memory area to fill
 * @val: Value to be set
 * @len: Number of bytes to be set to the value
 * 
 * Returns a pointer to the memory area dest.
 */
void *memset(void *dest, int val, size_t len);

/**
 * memcpy - Copy memory area
 * @dest: Destination memory area
 * @src: Source memory area
 * @len: Number of bytes to copy
 * 
 * Returns a pointer to dest.
 */
void *memcpy(void *dest, const void *src, size_t len);

#endif