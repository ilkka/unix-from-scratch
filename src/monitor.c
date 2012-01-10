/* monitor.c -- implementation of text mode monitor interface
 */

#include "monitor.h"
#include "common.h"

static u8int *video_memory = 0xB8000;

static u16int cursor_x = 0, cursor_y = 0;

static const u16int VGA_COMMAND_PORT = 0x3D4;
static const u16int VGA_DATA_PORT = 0x3D5;

static const u8int CURSOR_HI = 14;
static const u8int CURSOR_LO = 15;

// Update hardware cursor location
static void move_cursor()
{
	u16int cursorLocation = cursor_y * 80 + cursor_x;
	outb(VGA_COMMAND_PORT, CURSOR_HI);
	outb(VGA_DATA_PORT, cursorLocation >> 8);
	outb(VGA_COMMAND_PORT, CURSOR_LO);
	outb(VGA_DATA_PORT, cursorLocation);
}

// Scroll text up by one line
static void scroll()
{
	// Make a whitespace character with default attributes
	u8int attributeByte = (0 /* black */ << 4) | (15 /* white */ & 0x0F);
	u16int blankChar = 0x20 /* ascii space */ | (attributeByte << 8);

	// scroll up if cursor is on 25th row
	if (cursor_y >= 25)
	{
		// move currently visible text up one line
		int i;
		for (i = 0*80; i < 24*80; i++)
		{
			video_memory[i] = video_memory[i+80];
		}

		// now make last line blank by writing spaces
		for (i = 24*80; i < 25*80; i++)
		{
			video_memory[i] = blankChar;
		}

		// put cursor on last line
		cursor_y = 24;
	}
}

void monitor_put(char c)
{
	u8int bgColor = 0;
	u8int fgColor = 15;

	u8int attributeByte = (bgColor << 4) | (fgColor & 0x0F);
	u16int attribute = attributeByte << 8;
	u16int *location;

	// is it a backspace and are we not on the first column?
	if (c == 0x08 && cursor_x)
	{
		// yes, move one space left
		cursor_x--;
	}

	// Is it a tab?
	else if (c == 0x09)
	{
		// yes, jump to next multiple of 8
		cursor_x = (cursor_x + 8) & ~(8-1);
	}

	// Is it a carriage return?
	else if (c == '\r')
	{
		cursor_x = 0;
	}

	// Is it a newline?
	else if (c == '\n')
	{
		cursor_x = 0;
		cursor_y++;
	}

	// Any other printable characters
	else if (c >= ' ')
	{
		location = video_memory + (cursor_y*80 + cursor_x);
		*location = c | attribute;
		cursor_x++;
	}

	// is it the end of the screen?
	if (cursor_x >= 80)
	{
		cursor_x = 0;
		cursor_y++;
	}

	// scroll if necessary
	scroll();
	// move hw cursor
	move_cursor();
}

void monitor_clear();
{
	u8int bgColor = 0;
	u8int fgColor = 15;

	u8int attributeByte = (bgColor << 4) | (fgColor & 0x0F);
	u16int blank = 0x20 | (attributeByte << 8);

	int i;
	for (i = 0; i < 80*25; i++)
	{
		video_memory[i] = blank;
	}

	// cursor to top left
	cursor_x = 0;
	cursor_y = 0;
	move_cursor();
}

void monitor_write(char *c)
{
	int i = 0;
	while (c[i])
	{
		monitor_put(c[i++]);
	}
}


