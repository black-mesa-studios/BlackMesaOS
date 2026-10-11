#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../theweirdstuff/kbhandler.h"
#include "../gfx/con.h"

char getc(int echo) {
    char c = get_char();
    clear_keybuffer();
    if(echo && c && c != '\b') {
        putc(c);
    }
    return c;
}

void read(char* dst, size_t length, int echo) {
    size_t amount = 0;

    if (length == 0)
        return;

    while (true) {
        __asm__ volatile("hlt");

        char c = getc(0);

        if (!c)
            continue;

        if (c == '\n') {
            if (echo)
                putc('\n');
            break;
        }

        if (c == '\b') {
            if (amount > 0) {
                amount--;

                dst[amount] = '\0';

                if (echo)
                    backspace();
            }

            continue;
        }

        if (amount < length - 1) {
            dst[amount] = c;
            amount++;

            if (echo)
                putc(c);
        }
    }

    dst[amount] = '\0';
}