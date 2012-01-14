%macro ISR_NOERRCODE 1		; 1 param macro
[GLOBAL isr%1]
isr%1:
	cli			; no interrupts
	push	byte 0		; push dummy error code, isr doesn't
	push	byte %1		; push interrupt number
	jmp	isr_common_stub	; jump to handler
%endmacro

%macro ISR_ERRCODE 1		; another macro for isr's that push an error
[GLOBAL isr%1]
isr%1:
	cli
	push	byte %1
	jmp	isr_common_stub
%endmacro

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE 8
ISR_NOERRCODE 9
ISR_ERRCODE 10
ISR_ERRCODE 11
ISR_ERRCODE 12
ISR_ERRCODE 13
ISR_ERRCODE 14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

; defined in isr.c
[EXTERN isr_handler]

; common ISR stub: save processor state, set up for kernel mode, call c-level
; fault handler and restore stack frame
isr_common_stub:
	pusha			; push edi,esi,ebp,esp,ebx,edx,ecx,eax

	mov	ax, ds		; set low 16b if eax to ds
	push	eax		; save data segment descriptor

	mov	ax, 0x10	; kernel data segment descriptor
	mov	ds, ax
	mov	es, ax
	mov	fs, ax
	mov	gs, ax

	call	isr_handler

	pop	eax		; reload ds descriptor
	mov	ds, ax
	mov	es, ax
	mov	fs, ax
	mov	gs, ax

	popa			; pop what was pushed at the beginning
	add	esp, 8		; clean up pushed errcode and pushed ISR num
	sti
	iret			; pops cs, eip, eflags, ss and esp

