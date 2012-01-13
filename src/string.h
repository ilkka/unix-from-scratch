/* string.h -- string manipulation interface
 */

#ifndef STRING_H
#define STRING_H

/**
 * Fill area of memory with constant byte.
 *
 * @param s pointer to filled memory.
 * @param c byte to write.
 * @param n number of bytes to write.
 * @return pointer to s.
 */
void *memset(void *s, u8int c, u32int n);

/**
 * Copy memory from one area to another.
 *
 * @param dest pointer to destination.
 * @param src pointer to source.
 * @param n number of bytes to copy.
 * @return pointer to dest.
 */
void *memcpy(void *dest, void *src, u32int n);

/**
 * Get length of zero-terminated ascii string in bytes.
 *
 * @param s the string.
 * @return length as 32-bit unsigned integer.
 */
u32int strlen(const char* s);

#endif // STRING_H

