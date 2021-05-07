#ifndef __CONSOLE_H__
#define __CONSOLE_H__

/*
 * This is the function called by printf to send its output to the screen. You
 * have to implement it in the kernel and in the user program.
 */
void console_putbytes(const char *s, int len);

// Initialiser le terminal
void console_init();

// Écrire un caractère à un endroit donné
void console_putchar_at(char c, int column, int row);

#endif
