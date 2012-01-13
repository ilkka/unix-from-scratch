/* string.c -- implementation of traditional string functions
 */
#include "common.h"
#include "string.h"

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

u32int strlen(const char* s)
{
	u32int len = 0;
	while (*s++ != 0)
		++len;
	return len;
}
