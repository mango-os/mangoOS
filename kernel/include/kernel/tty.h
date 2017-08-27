#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>

void terminal_initialize(void);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_drawcursor(int x , int y);
void terminal_displaycursor(state);
void terminal_cursorpos(int row, int column);

#endif
