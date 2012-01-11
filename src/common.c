/* common.c -- definitions for global utility functions
 */

#include "common.h"

void outb(u16int port, u8int value)
{
	asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

u8int inb(u16int port)
{
	u8int ret;
	asm volatile ("inb %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

u16int inw(u16int port)
{
	u16int ret;
	asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

void *memset(void *s, u8int c, u32int n)
{
	if (n > 0)
	{
		asm volatile (
				".L_MEMSET_LOOP:"
				"mov %[ch],(%[ptr]);"
				"inc %[ptr];"
				"dec %[count];"
				"jnz .L_MEMSET_LOOP"
				 :
				 : [count] "c" (n), [ch] "r" (c), [ptr] "r" (s) 
			     );
	}
	return s;
}

void *memcpy(void *dest, void *src, u32int n)
{
	if (n > 0)
	{
		asm volatile (
				".L_MEMCPY_LOOP:"
				"mov (%[s]),%%eax;"
				"inc %[s];"
				"mov %%eax,(%[d]);"
				"inc %[d];"
				"dec %[count];"
				"jnz .L_MEMCPY_LOOP"
				:
				: [count] "c" (n), [s] "r" (src), [d] "r" (dest)
				: "%eax"
			     );
	}
	return dest;
}


