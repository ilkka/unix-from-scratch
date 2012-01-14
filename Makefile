# MAkefile for tutorials from
# http://www.jamesmolloy.co.uk/tutorial_html/1.-Environment%20setup.html

ASM_SRC = src/boot.s \
	  src/gdt.s \
	  src/idt.s \
	  src/interrupt.s

C_SRC = src/main.c \
	src/common.c \
	src/string.c \
	src/printf.c \
	src/monitor.c \
	src/descriptor_tables.c \
	src/isr.c

KERNEL=bin/kernel

CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector
LDFLAGS = -melf_i386 -Tlink.ld
ASFLAGS = -felf

all: link image run

SOURCES = $(ASM_SRC) $(C_SRC)
ASM_OBJS = $(patsubst src/%.s,obj/%.o,$(ASM_SRC))
C_OBJS = $(patsubst src/%.c,obj/%.o,$(C_SRC))
OBJS = $(ASM_OBJS) $(C_OBJS)

DEPS = $(patsubst src/%.c,.deps/%.d,$(C_SRC))
-include $(DEPS)

image: link
	./update_image.sh

run: image
	./run_bochs.sh

obj:
	mkdir obj

clean:
	rm -f $(OBJS)$(KERNEL)

link: obj $(OBJS) bin
	ld $(LDFLAGS) -o $(KERNEL) $(OBJS)

bin:
	mkdir bin

.PHONY: image link run clean all

obj/%.o: src/%.s
	nasm $(ASFLAGS) $<
	mv $(patsubst %.s,%.o,$<) $@

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

# MF: write rule to file
# MG: assume missing headers will be generated, don't barf
# MM: generate dependency rule for prerequisite, skip system headers
# MP: add phony target for each header to prevent missing header errors
# MT: add target for generated dependency
.deps/%.d: src/%.c
	mkdir -p .deps
	$(CC) $(CFLAGS) -MF"$@" -MG -MM -MP -MT"$@" -MT"$(patsubst src/%.c,obj/%.o,$<)" "$<"

