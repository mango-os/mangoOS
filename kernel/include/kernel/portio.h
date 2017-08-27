#ifndef _KERNEL_IOPORT_H
#define _KERNEL_IOPORT_H

#define NULL ((void *)0UL)

#if !(defined(__i386__) || defined(__x86_64__))
#error "This hardware platform doesn't have IO ports"
#endif

#include <stdint.h>

__attribute__((unused))
static inline uint8_t outport8(uint16_t port, uint8_t value)
{
	asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
	return value;
}

__attribute__((unused))
static inline uint16_t outport16(uint16_t port, uint16_t value)
{
	asm volatile ("outw %1, %0" : : "dN" (port), "a" (value));
	return value;
}

__attribute__((unused))
static inline uint32_t outport32(uint16_t port, uint32_t value)
{
	asm volatile ("outl %1, %0" : : "dN" (port), "a" (value));
	return value;
}

__attribute__((unused))
static inline uint8_t inport8(uint16_t port)
{
	uint8_t result;
	asm volatile("inb %1, %0" : "=a" (result) : "dN" (port));
	return result;
}

__attribute__((unused))
static inline uint16_t inport16(uint16_t port)
{
	uint16_t result;
	asm volatile("inw %1, %0" : "=a" (result) : "dN" (port));
	return result;
}

__attribute__((unused))
static inline uint32_t inport32(uint16_t port)
{
	uint32_t result;
	asm volatile("inl %1, %0" : "=a" (result) : "dN" (port));
	return result;
}

__attribute__((unused))
static inline outportsl(unsigned short port, unsigned char * data, unsigned long size)
{
	asm volatile ("rep outsw" : "+S" (data), "+c" (size) : "d" (port));
}

__attribute__((unused))
static inline inportsl(unsigned short port, unsigned char * data, unsigned long size)
{
	asm volatile ("rep insw" : "+D" (data), "+c" (size) : "d" (port) : "memory");
}

__attribute__((unused))
static inline void io_wait(void)
{
	asm volatile("outb %%al, $0x80" : :"a"(0) );
}

__attribute__((unused))
static inline void cli()
{
	__asm__ __volatile__ ("cli"); //Off
}

__attribute__((unused))
static inline void sti()
{
	__asm__ __volatile__ ("sti"); //On
}

#endif
