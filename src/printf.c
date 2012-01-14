/* printf.c -- formatted output.
 *
 * This stuff is more or less copied from Linux 0.11's kernel/vsprintf.c
 * and is Copyright Linus Torvalds. It is used here according to Linux's
 * licence at the time, which allows redistribution in part or whole as
 * long as the whole source is available free of charge or any fee.
 */

#include "common.h"
#include "printf.h"
#include "string.h"
#include "monitor.h"

// Output flags definitions
#define LEFT (1 << 0)
#define PLUS (1 << 1)
#define SPACE (1 << 2)
#define SPECIAL (1 << 3)
#define ZEROPAD (1 << 4)
#define SMALL (1 << 5)
#define SIGN (1 << 6)

// Decimal digit check
#define is_digit(c) ((c) >= '0' && (c) <= '9')

// divide number by base and yield the remainder
static int do_div(int *n, int base) {
	int res = *n % base;
	*n /= base;
	return res;
}

/**
 * Read decimal digits, convert to int and return while advancing the ptr.
 *
 * @param s pointer to pointer to string. The target pointer to string
 * will be advanced past the end of the number (the last decimal digit in a
 * row).
 */
static int skip_atoi(const char **s)
{
	int i = 0;
	while (is_digit(**s)) {
		i = (10 * i) + *((*s)++) - '0';
	}
	return i;
}

/**
 * Output number num into buffer str with base base, field width size,
 * precision precision, and return a pointer to the next position in
 * the buffer.
 *
 * @param str the buffer.
 * @param num number to output.
 * @param base base to use.
 * @param size field width.
 * @param precision precision.
 * @param type flags go here.
 * @return pointer to one past the end of the number as output in the buffer,
 * or zero on error.
 */
static char *number(char *str, int num, int base, int size, int precision,
		int type)
{
	char c,		// blank for filling
	     sign,	// sign character or zero for none
	     tmp[36];	// build number string here
	const char *digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int i;

	if (type & SMALL) digits = "0123456789abcdefghijklmnopqrstuvwxyz";
	if (type & LEFT) type &= ~ZEROPAD; // can't do both
	if (base < 2 || base > 36)	// sorry, can't help ya.
		return 0;

	c = (type & ZEROPAD) ? '0' : ' ';

	if (type & SIGN && num < 0) {
		sign = '-';
		num = -num;
	} else {
		sign = (type & PLUS) ? '+' : ((type & SPACE) ? ' ' : 0);
	}

	if (sign) size--;	// sign eats one

	if (type & SPECIAL) {
		if (base == 16) size -= 2;	// for '0x'
		else if (base == 8) size--;	// for '0'
	}

	i=0;

	if (num == 0) {
		tmp[i++] = '0';
	} else {
		while (num != 0) {
			// insert next digit
			tmp[i++] = digits[do_div(&num, base)];
		}
	}
	
	if (i > precision) precision = i;	// overridden
	size -= precision;

	// Pad on the left?
	if (!(type & (ZEROPAD + LEFT))) {
		while (size-- > 0)
			*str++ = ' ';
	}

	if (sign)
		*str++ = sign;

	if (type & SPECIAL) {
		if (base == 8) {
			*str++ = '0';
		} else if (base == 16) {
			*str++ = '0';
			*str++ = digits[33];	// big or little x
		}
	}

	if (!(type & LEFT)) {
		while (size-- > 0) *str++ = c;
	}

	// zeros til the end
	while (i < precision--) *str++ = '0';

	// stick it in the string
	while (i-- > 0) *str++ = tmp[i];

	// spaces for the rest until the field width is up
	while (size-->0) *str++ = ' ';

	return str;
}

int sprintf(char *buf, const char *fmt, va_list args)
{
	char *str;	// for keeping track of output location in buf
	int flags;	// for keeping track of formatting flags
	int field_width;// width of output field
	int precision;	// min number of digits for integers; for strings,
			// max number of chars
	int qualifier;	// 'h', 'l' or 'L' for integers
	char *s;	// if there's a string argument, this is it
	int len;	// this is its length
	int i;		// index variable
	int *ip;	// ptr for 'n' format

	for (str = buf; *fmt; ++fmt) { // loop til end of format string
		if (*fmt != '%') { // detect speshul placeholders
			*str++ = *fmt; // twas a regular char so stick it in
			continue;
		}

		flags = 0;
repeat: // an elegant use of goto if I've ever seen one
		++fmt;		// look at next char, or skip first '%'
		switch(*fmt) {	// look at it!
			case '-': flags |= LEFT; goto repeat;
			case '+': flags |= PLUS; goto repeat;
			case ' ': flags |= SPACE; goto repeat;
			case '#': flags |= SPECIAL; goto repeat;
			case '0': flags |= ZEROPAD; goto repeat;
		}

		// flags done, now field width
		field_width = -1;
		if (is_digit(*fmt)) {
			field_width = skip_atoi(&fmt);
		} else if (*fmt == '*') { // width in next arg
			field_width = va_arg(args, int);
			if (field_width < 0) {
				// negative width means left-align
				field_width = -field_width;
				flags |= LEFT;
			}
		}

		// width done, get precision
		precision = -1;
		if (*fmt == '.') { // precision coming up
			++fmt;
			if (is_digit(*fmt)) {
				precision = skip_atoi(&fmt);
			} else if (*fmt == '*') {
				// precision is next argument
				precision = va_arg(args, int);
			}
			if (precision < 0) precision = 0;
		}

		// conversion qualifier
		qualifier = -1;
		if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L') {
			qualifier = *fmt;
			++fmt;
		}

		// formats
		switch(*fmt) {
			case 'c':
				if (!(flags & LEFT)) {
					while (--field_width > 0) {
						*str++ = ' ';
					}
				}
				*str++ = (unsigned char) va_arg(args, int);
				while (--field_width > 0) {
					*str++ = ' ';
				}
				break;
			case 's':
				s = va_arg(args, char*);
				len = strlen(s);
				// Set precision, truncate string if necessary
				if (precision < 0)
					precision = len;
				else if (len > precision)
					len = precision;
				// left-pad
				if (!(flags & LEFT)) {
					while (len < field_width--) {
						*str++ = ' ';
					}
				}
				for (i = 0; i < len; ++i) {
					*str++ = *s++;	// output!
				}
				// right-pad
				while (len < field_width--) {
					*str++ = ' ';
				}
				break;
			case 'o': // octal
				str = number(str, va_arg(args, u32int),
						8, field_width, precision, flags);
				break;
			case 'p': // pointer output
				if (-1 == field_width) {
					field_width = 8;
					flags |= ZEROPAD;
				}
				str = number(str, (u32int) va_arg(args, void*),
						16, field_width, precision, flags);
				break;
			case 'x':
				flags |= SMALL;
			case 'X':
				str = number(str, va_arg(args, u32int),
						16, field_width, precision, flags);
				break;
			case 'd':
			case 'i':
				flags |= SIGN;
			case 'u':
				str = number(str, va_arg(args, unsigned long), 10,
						field_width, precision, flags);
				break;
			case 'n': // actually no idea what this is :--D
				ip = va_arg(args, int*);
				*ip = (str - buf);
				break;
			default:
				if (*fmt != '%') // literal % wanted
					*str++ = '%';
				if (*fmt)	// fallthrough for non-zero
					*str++ = *fmt;
				else
					--fmt;
				break;
		}
	}
	*str = '\0';
	return str-buf; // str is now at end of output buffer
}

void printf(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	static char buf[4096];
	sprintf(buf, fmt, args);
	monitor_write(buf);
}
