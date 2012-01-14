; asm function for writing GDT pointer
; from JamesM's UNIX tutorial
[GLOBAL gdt_flush]

gdt_flush:
	mov	eax, [esp+4]		; get pointer to GDT from stack
	lgdt	[eax]			; load GDT from pointer in EAX

	mov	ax, 0x10		; 0x10 is offset in GDT to data seg
	mov	ds, ax			; load segment registers
	mov	es, ax
	mov	fs, ax
	mov	gs, ax
	mov	ss, ax

	jmp	0x08:.flush		; far jump to code segment
.flush:
	ret

