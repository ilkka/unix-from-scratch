/* descriptor_tables.h - structs and typedefs for GDT and IDT
 * table entries
 */
#ifndef DESCRIPTOR_TABLES_H
#define DESCRIPTOR_TABLES_H

/**
 * GDT entry struct definition
 *
 * Access flags byte layout:
 *
 * +---+------+----+------+
 * | P | DPL  | DT | Type |
 * +---+------+----+------+
 *   7   6..5   4    3..0
 *
 * P: Is segment present? (1=yes)
 * DPL: Descriptor privilege level (ring) 0-3
 * DT: Descriptor type (?)
 * Type: Segment type (code or data)
 *
 * Granularity byte layout:
 *
 * +---+---+---+---+----------------+
 * | G | D | 0 | A | Segment length |
 * +---+---+---+---+----------------+
 *   7   6   5   4   3............0
 *
 * G: Granularity (0=byte, 1=kilobyte)
 * D: Operand size (0=16b, 1=32b)
 * 0: Always zero
 * A: Available for system use (always zero)
 * Segment length:
 *
 */
struct gdt_entry_struct
{
    u16int limit_low;   // low 16 bits of limit of this segment
    u16int base_low;    // low 16 bits of base of this segment
    u8int base_middle;  // next 8 bits of base
    u8int access;       // access flags byte
    u8int granularity;  // granularity flags byte
    u8int base_high;    // last 8 bits of base
} __attribute((packed)); // don't screw with my alignment, yo
typedef struct gdt_entry_struct gdt_entry_t;

/**
 * Pointer struct for giving processor GDT location
 */
struct gdt_ptr_struct
{
    u16int limit;       // upper 16 bits of all selector limits
    u32int base;        // address of first gdt_entry_t struct
} __attribute((packed));
typedef struct gdt_ptr_struct gdt_ptr_t;

/**
 * Initialize GDT and IDT, define default ISR and IRQ handlers.
 */
void init_descriptor_tables();

/**
 * Interrupt descriptor table entry.
 *
 * Flags byte layout:
 *
 * +---+------+--------------------+
 * | P | DPL  | Always 00110 == 14 |
 * +---+------+--------------------+
 *   7   6..5   4................0
 */
struct idt_entry_struct
{
    u16int base_lo;         // low 16 bits of address of handler
    u16int sel;             // kernel segment selector
    u8int always0;          // always zero
    u8int flags;            // flags, beautiful flags
    u16int base_high;       // high 16 bits of address to handler
} __attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

/**
 * Pointer to an IDT struct
 */
struct idt_ptr_struct
{
    u16int limit;
    u32int base;            // address of first element in idt_entry_t array
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

// Extern declarations so that asm can access the addresses of ISR handlers
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

#endif // DESCRIPTOR_TABLES_H

