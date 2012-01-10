# MAkefile for tutorials from
# http://www.jamesmolloy.co.uk/tutorial_html/1.-Environment%20setup.html

SOURCES=boot.o main.o

CFLAGS=-m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector
LDFLAGS=-melf_i386 -Tlink.ld
ASFLAGS=-felf

all: $(SOURCES) link

clean:
	-rm *.o kernel

link:
	ld $(LDFLAGS) -o kernel $(SOURCES)

%.o: %.s
	nasm $(ASFLAGS) $<
