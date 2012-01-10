/* Linker script for unix from scratch tutorial from
 * http://www.jamesmolloy.co.uk/tutorial_html/1.-Environment%20setup.html
 */

OUTPUT_FORMAT(elf32-i386)
ENTRY(start) /* start location for binary is 'start' */
SECTIONS
{
	/* '.text' section should be first in the image, starting at 1 MB */
	.text 0x100000 :
	{
		code = .; _code = .; __code = .; /* TODO: what's this do? */
		*(.text)
		. = ALIGN(4096); /* page-aligned */
	}

	/* '.data' section is next */
	.data :
	{
		data = .; _data = .; __data = .;
		*(.data)
		*(.rodata) /* linux GCC adds read-only data, put it here too */
		. = ALIGN(4096);
	}

	/* '.bss' section is last */
	.bss :
	{
		bss = .; _bss = .; __bss = .;
		*(.bss)
		. = ALIGN(4096);
	}

	/* here it ends */
	end = .; _end = .; __end = .;
}
