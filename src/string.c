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

int strlen(const char* s)
{
	int len = 0;
	asm(
			"cld;"	// clear DF: strings iterd from lo to hi addr
			"repne;" // repeat while not equal (zf != 1)
			"scasb;" // with above, find AL in string at ES:DI
			"notl %[len];" // one's complement?
			"decl %[len]"
			: [len] "=c" (len) // length is set in CX
			: "D" (s), // put string addr into DI
			  "a" (0), // init AL to zero
			  "0" (0xffffffff) // start length at int_min
	   );
	return len;
}
