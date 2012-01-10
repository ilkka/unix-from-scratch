/* monitor.h -- define interface for monitor
 */

#ifndef MONITOR_H
#define MONITOR_H

#include "common.h"

/**
 * Write one character to screen.
 *
 * @param c the character.
 */
void monitor_put(char c);

/**
 * Clear screen to all black.
 */
void monitor_clear();

/**
 * Output null-terminated ASCII string to monitor.
 *
 * @param c the string.
 */
void monitor_write(char *c);

#endif // MONITOR_H

