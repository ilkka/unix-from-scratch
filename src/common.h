/* common.h -- utilities and global types
 */

#ifndef COMMON_H
#define COMMON_H

// Integers with known sizes
// TODO: really should verify these somehow compile-time
typedef unsigned    int     u32int;
typedef             int     s32int;
typedef unsigned    short   u16int;
typedef             short   s16int;
typedef unsigned    char    u8int;
typedef             char    s8int;

/**
 * Output byte to port.
 *
 * @param port the port.
 * @param value the byte.
 */
void outb(u16int port, u8int value);

/**
 * Input byte from port.
 *
 * @param port the port.
 * @return the byte.
 */
u8int inb(u16int port);

/**
 * Input word from port.
 *
 * @param port the port.
 * @return the word.
 */
u16int inw(u16int port);

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

#endif // COMMON_H

