#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <inttypes.h>
#include <n7OS/console.h>

// Couleurs du terminal
enum color {
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15,
};

static const size_t TERMINAL_WIDTH = 80;
static const size_t TERMINAL_HEIGHT = 25;

uint16_t *TERMINAL_SRC = (uint16_t*) 0xb8000; // Zone mémoire de la console
uint8_t *CURSOR_COMMAND_PORT = (uint8_t*) 0x3d4; // Port de commande du curseur
uint8_t *CURSOR_VALUE_PORT = (uint8_t*) 0x3d5; // Port de valeur du curseur

enum color terminal_color_fg = COLOR_WHITE; // Couleur d'écriture
enum color terminal_color_bg = COLOR_BLACK; // Couleur de fond du terminal
int terminal_row = 0; // Ligne du curseur
int terminal_column = 0; // Colonne du curseur


// Obtenir les 2 octets à placer dans la zone mémoire de la console pour afficher un caractère en couleur
uint16_t console_entry(unsigned char c, enum color bg, enum color fg) {
	uint8_t color = fg | bg << 4;
	return (uint16_t) c | (uint16_t) color << 8;
}


// Écrire un caractère à un endroit donné
void console_putchar_at(char c, int column, int row) {
	int index = row * TERMINAL_WIDTH + column;
	TERMINAL_SRC[index] = console_entry(c, terminal_color_bg, terminal_color_fg);
}


// Écrire un caractère à la suite des précédentes écritures
void console_putchar(char c) {
	console_putchar_at(c, terminal_column, terminal_row);
	terminal_column++;
	if (terminal_column == TERMINAL_WIDTH) {
		terminal_column = 0;
		terminal_row++;
		if (terminal_row == TERMINAL_HEIGHT)
			terminal_row = 0;
	}
}


// Initialiser le terminal
void console_init() {
	for (int row = 0; row < TERMINAL_HEIGHT; row++) {
		for (int column = 0; column < TERMINAL_WIDTH; column++) {
			console_putchar_at(' ', column, row);
		}
	}
}


void console_putbytes(const char *s, int len) {
	for (int i = 0; i < len; i++)
		console_putchar(s[i]);
}
