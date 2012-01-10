; boot.s -- this is where the kernel starts running

MBOOT_PAGE_ALIGN	equ 1<<0	; load on page boundary
MBOOT_MEM_INFO		equ 1<<1	; provide kernel with memory info
MBOOT_HEADER_MAGIC	equ 0x1BADB002	; multiboot magic value
; Not using MBOOT_AOUT_KLUDGE, which means that GRUB does not give us
; a symbol table.
MBOOT_HEADER_FLAGS	equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM		equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

[BITS 32]				; 32-bit instructions throughout

[GLOBAL mboot]				; Make 'mboot' accessible from C
[EXTERN code]				; Start of .text section
[EXTERN bss]				; Start of .bss section
[EXTERN end]				; End of last loadable section

; The multiboot descriptor
mboot:
	dd	MBOOT_HEADER_MAGIC	; add this constant here, grub
					; searches for it.
	dd	MBOOT_HEADER_FLAGS	; instructions for grub on how to load
	dd	MBOOT_CHECKSUM		; grub uses this to verify the load

	dd	mboot			; location of this descriptor
	dd	code			; start of code or .text section
	dd	bss			; end of kernel .data section
	dd	end			; end of kernel
	dd	start			; kernel entry point

[GLOBAL start]
[EXTERN main]				; entry point of C code

start:
	push	ebx			; Load multiboot header location
	; execute kernel
	cli				; interrupts off
	call	main			; call C main
	jmp	$			; infinite loop so we don't execute
					; from whatever memory after the kernel
