#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>
#include <kernel/portio.h>

#include "vga.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}

void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

static void terminal_scroll()
{
	uint16_t* buf = terminal_buffer;
	for ( size_t y = 1; y < VGA_HEIGHT; y++ )
		for ( size_t x = 0; x < VGA_WIDTH; x++ )
			buf[(y-1) * VGA_WIDTH + x] = buf[y * VGA_WIDTH + x];
	for ( size_t x = 0; x < VGA_WIDTH; x++ )
		buf[(VGA_HEIGHT-1) * VGA_WIDTH + x] = make_vgaentry(' ', terminal_color);
}

static void terminal_newline()
{
	terminal_column = 0;
	if ( terminal_row + 1 == VGA_HEIGHT )
		terminal_scroll();
	else
		terminal_row++;
}

void terminal_putchar(char c)
{
	if ( c == '\n' )
		terminal_newline();
	else if ( c == '\r' )
		terminal_column = 0;
	else if ( c == '\b' )
	{
		terminal_column--;
		terminal_writestring(" ");
		terminal_column--;
	}
	else
	{
		if ( terminal_column == VGA_WIDTH )
			terminal_newline();
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
		terminal_column++;
	}
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}

void terminal_drawcursor(int row , int column) {
	unsigned short cursorLocation = row * 80 + (column);
	outport8(0x3D4, 14);                 
	outport8(0x3D5, cursorLocation >> 8); 
	outport8(0x3D4, 15);                  
	outport8(0x3D5, cursorLocation); 
}

void terminal_displaycursor(state)
{
	//TODO on or off
}

void terminal_cursorpos(int row, int column)
{
		if (!row == -1)
		{
			terminal_row = row;
		}

		if (!column == -1)
		{
			terminal_column = column;
		}
}