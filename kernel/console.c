#include <stdbool.h>
#include <stddef.h>
#include <inttypes.h>
#include <n7OS/console.h>

/* Couleurs du terminal */
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
 
static inline uint8_t entry_color(enum color foreground, enum color background) 
{
	return foreground | background << 4;
}

static inline uint16_t entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

void console_putbytes(const char *s, int len) {
    uint16_t *src_tab = (uint16_t *) 0xb8000;
    int pos = 10*80 +3;
    for (int i = 0; i < len; i++) {
        src_tab[pos+i] = (0xB<<8)|s[i];
    }
}
