/* descriptor_tables.c -- implementation for descriptor tables stuff.
 * From JamesM's UNIX tutorial.
 */

#include "common.h"
#include "descriptor_tables.h"

/**
 * Flush GDT and load GDT pointer.
 *
 * Defined in asm, make available in C like this.
 *
 * @param p gdt_ptr properly initialized
 */
extern void gdt_flush(u32int p);

static void gdt_init();

/**
 * Set the value of one GDT entry.
 *
 * @param num number of the entry in the GDT
 * @param base base address
 * @param limit the limit
 * @param access the access byte
 * @param gran granularity byte
 */
static void gdt_set_gate(s32int num, u32int base, u32int limit,
		u8int access, u8int gran);

// Defined in asm
extern void idt_flush(u32int ptr);

/**
 * Initialize IDTs
 */
static void idt_init();

/**
 * Set the value of one IDT entry.
 *
 * @param num number of the entry in the IDT
 * @param base base address
 * @param sel kernel segment selector
 * @param flags flags byte.
 */
static void idt_set_gate(u8int num, u32int base, u16int sel, u8int flags);

idt_entry_t idt_entries[256];
idt_ptr_t idt_ptr;

gdt_entry_t gdt_entries[5]; // cs, ds, es, fs, gs, ss
gdt_ptr_t gdt_ptr;

// perform GDT and IDT initialization
void init_descriptor_tables()
{
	gdt_init();
	idt_init();
}

static void gdt_init()
{
	gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
	gdt_ptr.base = (u32int)&gdt_entries;

	gdt_set_gate(0, 0, 0, 0, 0); // null segment: must have or bad stuff
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // code segment
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // data segment
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // user mode code segment
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // user mode data segment

	gdt_flush((u32int)&gdt_ptr);
}

static void gdt_set_gate(s32int num, u32int base, u32int limit,
		u8int access, u8int gran)
{
	gdt_entries[num].base_low = (base & 0xFFFF);
	gdt_entries[num].base_middle = 0xFF & (base >> 16);
	gdt_entries[num].base_high = 0xFF & (base >> 24);
	gdt_entries[num].limit_low = (limit & 0xFFFF);
	gdt_entries[num].granularity = (limit >> 16) & 0x0F;
	gdt_entries[num].granularity |= gran & 0xF0;
	gdt_entries[num].access = access;
}

static void idt_init()
{
	idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
	idt_ptr.base = (u32int)&idt_entries;

	memset(&idt_entries, 0, sizeof(idt_entry_t)*256);

	idt_set_gate( 0, (u32int)isr0, 0x08, 0x8E);
	idt_set_gate( 1, (u32int)isr1, 0x08, 0x8E);
	idt_set_gate( 2, (u32int)isr2, 0x08, 0x8E);
	idt_set_gate( 3, (u32int)isr3, 0x08, 0x8E);
	idt_set_gate( 4, (u32int)isr4, 0x08, 0x8E);
	idt_set_gate( 5, (u32int)isr5, 0x08, 0x8E);
	idt_set_gate( 6, (u32int)isr6, 0x08, 0x8E);
	idt_set_gate( 7, (u32int)isr7, 0x08, 0x8E);
	idt_set_gate( 8, (u32int)isr8, 0x08, 0x8E);
	idt_set_gate( 9, (u32int)isr9, 0x08, 0x8E);
	idt_set_gate(10, (u32int)isr10, 0x08, 0x8E);
	idt_set_gate(11, (u32int)isr11, 0x08, 0x8E);
	idt_set_gate(12, (u32int)isr12, 0x08, 0x8E);
	idt_set_gate(13, (u32int)isr13, 0x08, 0x8E);
	idt_set_gate(14, (u32int)isr14, 0x08, 0x8E);
	idt_set_gate(15, (u32int)isr15, 0x08, 0x8E);
	idt_set_gate(16, (u32int)isr16, 0x08, 0x8E);
	idt_set_gate(17, (u32int)isr17, 0x08, 0x8E);
	idt_set_gate(18, (u32int)isr18, 0x08, 0x8E);
	idt_set_gate(19, (u32int)isr19, 0x08, 0x8E);
	idt_set_gate(20, (u32int)isr20, 0x08, 0x8E);
	idt_set_gate(21, (u32int)isr21, 0x08, 0x8E);
	idt_set_gate(22, (u32int)isr22, 0x08, 0x8E);
	idt_set_gate(23, (u32int)isr23, 0x08, 0x8E);
	idt_set_gate(24, (u32int)isr24, 0x08, 0x8E);
	idt_set_gate(25, (u32int)isr25, 0x08, 0x8E);
	idt_set_gate(26, (u32int)isr26, 0x08, 0x8E);
	idt_set_gate(27, (u32int)isr27, 0x08, 0x8E);
	idt_set_gate(28, (u32int)isr28, 0x08, 0x8E);
	idt_set_gate(29, (u32int)isr29, 0x08, 0x8E);
	idt_set_gate(30, (u32int)isr30, 0x08, 0x8E);
	idt_set_gate(31, (u32int)isr31, 0x08, 0x8E);

	idt_flush((u32int)&idt_ptr);
}

static void idt_set_gate(u8int num, u32int base, u16int sel, u8int flags)
{
	idt_entries[num].base_lo = base & 0xFFFF;
	idt_entries[num].base_high = (base >> 16) & 0xFFFF;

	idt_entries[num].sel = sel;
	idt_entries[num].always0 = 0;

	// for usermode we will want to uncomment the OR below...
	idt_entries[num].flags = flags /* | 0x60 */;
}
