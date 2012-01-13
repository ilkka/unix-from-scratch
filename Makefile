# MAkefile for tutorials from
# http://www.jamesmolloy.co.uk/tutorial_html/1.-Environment%20setup.html

OBJS=obj/boot.o \
     obj/main.o \
     obj/common.o \
     obj/monitor.o \
     obj/string.o \
     obj/printf.o

KERNEL=bin/kernel

CFLAGS=-m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector
LDFLAGS=-melf_i386 -Tlink.ld
ASFLAGS=-felf

all: obj $(OBJS) link

obj:
	-mkdir obj

clean:
	-rm *.o $(KERNEL)

link: bin
	ld $(LDFLAGS) -o $(KERNEL) $(OBJS)

bin:
	-mkdir bin

obj/%.o: src/%.s
	nasm $(ASFLAGS) $<
	mv $(patsubst %.s,%.o,$<) $@

obj/%.o: src/%.c
	gcc $(CFLAGS) -o $@ -c $<
