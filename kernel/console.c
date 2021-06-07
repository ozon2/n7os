#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <inttypes.h>
#include <n7OS/console.h>
#include <n7OS/cpu.h>
#include <n7OS/time.h>

#define CURSOR_COMMAND_PORT 0x3D4 // Port de commande du curseur
#define CURSOR_VALUE_PORT 0x3D5 // Port de valeur du curseur
#define TERMINAL_WIDTH 80 // Largeur du terminal
#define TERMINAL_HEIGHT 25 // Hauteur du terminal

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
	COLOR_YELLOW = 14,
	COLOR_WHITE = 15,
};



uint16_t *TERMINAL_SRC = (uint16_t*) 0xB8000; // Zone mémoire de la console

enum color terminal_color_fg = COLOR_WHITE; // Couleur d'écriture
enum color terminal_color_bg = COLOR_LIGHT_BLUE; // Couleur de fond du terminal
int cursor_row; // Ligne du curseur
int cursor_column; // Colonne du curseur

// Positionner le curseur
void cursor_setpos(uint16_t pos) {
	outb(0xF, CURSOR_COMMAND_PORT); // Envoi de la commande 15 (0xF)
	outb((uint8_t) (pos & 0xFF), CURSOR_VALUE_PORT); // Envoi de la valeur de poids faible de la position
	outb(0xE, CURSOR_COMMAND_PORT); // Envoi de la commande 14 (0xE)
	outb((uint8_t) ((pos >> 8) & 0xFF), CURSOR_VALUE_PORT); // Envoi de la valeur de poids fort
}


// Obtenir les 2 octets à placer dans la zone mémoire de la console pour afficher un caractère en couleur
uint16_t console_entry(unsigned char c, enum color bg, enum color fg) {
	uint8_t color = fg | bg << 4;
	return (uint16_t) c | (uint16_t) color << 8;
}


// Écrire un caractère à un endroit donné
void console_putchar_at(char c, int column, int row) {
	uint16_t pos = row * TERMINAL_WIDTH + column;
	TERMINAL_SRC[pos] = console_entry(c, terminal_color_bg, terminal_color_fg);
}


// Initialiser le terminal
void console_init() {
	cursor_setpos(0);
	for (int row = 0; row < TERMINAL_HEIGHT; row++) {
		for (int column = 0; column < TERMINAL_WIDTH; column++) {
			console_putchar_at(' ', column, row);
		}
	}
	cursor_column = 0;
	cursor_row = 0;
}


// Déplacer l'affichage vers le haut pour afficher la nouvelle ligne
void scroll() {
	memcpy(TERMINAL_SRC, TERMINAL_SRC + TERMINAL_WIDTH, 2*TERMINAL_WIDTH*(TERMINAL_HEIGHT - 1));
	for (int i = 0; i < TERMINAL_WIDTH; i++) {
		console_putchar_at(' ', i, TERMINAL_HEIGHT-1);
	}
	show_timer(); // Réafficher le timer après avoir scrollé
}


// Écrire un caractère et mettre à jour la position du curseur
void console_putchar(const char c) {
	if (c > 31 && c < 127) { // Caractères affichables
		console_putchar_at(c, cursor_column, cursor_row);
		cursor_column++;
	} else if (c == '\n') { // Déplace le curseur à la ligne suivante, colonne 0
		cursor_row++;
		cursor_column = 0;
	} else if (c == '\b') { // Déplace le curseur d'une colonne en arrière
		cursor_column--;
	} else if (c == '\t') { // Ajoute un espace de 8 caractères
		cursor_column += 8;
	} else if (c == '\f') { // Efface l'écran et revient à la colonne 0 de la ligne 0
		console_init();
	} else if (c == '\r') { // Déplace le curseur à la colonne 0 de la ligne courante
		cursor_column = 0;
	}

	if (cursor_column == TERMINAL_WIDTH) { // Aller à la ligne suivante si on est à la fin de la ligne
		cursor_column = 0;
		cursor_row++;
	}
	if (cursor_row == TERMINAL_HEIGHT) { // Scroller si on arrive en bas de l'écran
		scroll();
		cursor_row--;
	}

	uint16_t pos = cursor_row * TERMINAL_WIDTH + cursor_column;
	cursor_setpos(pos);
}


void console_putbytes(const char *s, int len) {
	for (int i = 0; i < len; i++)
		console_putchar(s[i]);
}
