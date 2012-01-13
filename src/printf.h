/* printf.h -- formatted output functions.
 */
#ifndef PRINTF_H
#define PRINTF_H

/**
 * Formatted output into string buffer.
 *
 * @param buf the string buffer.
 * @param fmt the format string.
 * @param args arguments for format string.
 * @return number of character written.
 */
int sprintf(char *buf, const char *fmt, va_list args);

/**
 * Formatted output to screen.
 *
 * @param fmt the format string.
 * @param args arguments for format string.
 */
void printf(const char *fmt, va_list args);

#endif // PRINTF_H
