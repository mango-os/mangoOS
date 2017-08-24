DEFAULT_HOST := i686-elf
HOST := $(DEFAULT_HOST)
HOSTARCH := i386

AR := $(HOST)-ar
AS := $(HOST)-as
CC := $(HOST)-gcc

PREFIX := /usr
EXEC_PREFIX := /usr
BOOTDIR := /boot
LIBDIR := $(EXEC_PREFIX)/lib
INCLUDEDIR := $(PREFIX)/include

CFLAGS := -O2 -g

ROOTDIR := $(shell pwd)
SYSROOT := $(ROOTDIR)/sysroot
DESTDIR := $(SYSROOT)

CC := $(CC) --sysroot=$(SYSROOT) -isystem=$(INCLUDEDIR)

PROJECTS := libc kernel
SYSTEM_HEADERS_PROJECRS := libc kernel

export

.PHONY: clean build install-headers qemu

help:
	@echo "Usage: 	make [ all | build | help | clean | run | help ]" 
	@echo "	all:	编译运行"
	@echo "	build:	编译"
	@echo "	run:	运行"
	@echo "	clean:	清理"
	@echo "	help:	帮助"

all:clean build run

run: mangoOS.iso
	qemu-system-$(HOSTARCH) -cdrom mangoOS.iso

build: install-headers
	$(MAKE) -C libc install
	$(MAKE) -C kernel install
	mkdir -p isodir
	mkdir -p isodir/boot
	mkdir -p isodir/boot/grub
	cp sysroot/boot/mangoOS.kernel isodir/boot/mangoOS.kernel
	echo 'menuentry "mangoOS" {multiboot /boot/mangoOS.kernel}' > isodir/boot/grub/grub.cfg
	grub-mkrescue -o mangoOS.iso isodir

install-headers:
	mkdir -p $(SYSROOT)
	$(MAKE) -C libc install-headers
	$(MAKE) -C kernel install-headers

clean:
	$(MAKE) -C libc clean
	$(MAKE) -C kernel clean
	rm -rf sysroot isodir mangoOS.iso
