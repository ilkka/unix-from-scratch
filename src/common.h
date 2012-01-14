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

/**************************************************************************
 * Variable length argument list support macros
 *
 * These are more or less pilfered from Linux 0.11 include/stdarg.h and are
 * Copyright Linux Torvalds. They are used in accordance with Linux's
 * licence at the time, which stated that redistribution in whole or part
 * is allowed as long as full source is made available and no fees are
 * charged.
 **************************************************************************/

typedef char *va_list;

/**
 * Round the size of expression TYPE to next multiple of sizeof(int).
 */
#define __va_rounded_size(TYPE) \
    (((sizeof(TYPE) + sizeof(int)-1) / sizeof(int)) * sizeof(int))

/**
 * Define AP to point at the first variable arg
 */
#define va_start(AP, LASTARG) \
     AP = ((char *) &(LASTARG) + __va_rounded_size(LASTARG))

/**
 * Get next arg as type TYPE and advance the pointer
 */
#define va_arg(AP, TYPE) \
    (AP += __va_rounded_size(TYPE), \
     *((TYPE*)(AP - __va_rounded_size(TYPE))))

#endif // COMMON_H

