#include "vga.h"
#include <stdarg.h>

static int cursorX = 0;
static int cursorY = 0;

#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_MEMORY ((volatile uint16_t*)0xB8000)

static void doomscroll(void)
{
    for (int y = 1; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            VGA_MEMORY[(y - 1) * VGA_WIDTH + x] =
                VGA_MEMORY[y * VGA_WIDTH + x];
        }
    }
    for (int x = 0; x < VGA_WIDTH; x++) {
        VGA_MEMORY[(VGA_HEIGHT - 1) * VGA_WIDTH + x] =
            (uint16_t)0x0700 | ' ';
    }
}

void makesurecoordsaresafe_exe(void)
{
    if (cursorX < 0)
        cursorX = 0;

    if (cursorY < 0)
        cursorY = 0;

    if (cursorX >= VGA_WIDTH) {
        cursorX = 0;
        cursorY++;
    }

    while (cursorY >= VGA_HEIGHT) {
        doomscroll();
        cursorY--;
    }

    VGA_CURSOR_MOVE(cursorX, cursorY);
}

void backspace()
{
    if (cursorX == 0 && cursorY == 0)
        return;
    if (cursorX == 0) {
        cursorX = VGA_WIDTH - 1;
        cursorY--;
    } else {
        cursorX--;
    }
    VGA_WRITE(' ', cursorX, cursorY);
    VGA_CURSOR_MOVE(cursorX, cursorY);
}

void putc(char c) {
    if (c == '\n') {
        cursorX = 0;
        cursorY++;
    }
    else {
        VGA_WRITE(c, cursorX, cursorY);
        cursorX++;
    }
    makesurecoordsaresafe_exe();
    VGA_CURSOR_MOVE(cursorX, cursorY);
}

void pr_int(int n)
{
    unsigned int num;

    if (n < 0)
    {
        putc('-');
        num = -(unsigned int)n;
    }
    else
        num = (unsigned int)n;

    if (num >= 10)
        pr_int(num / 10);

    putc('0' + (num % 10));
}

void simple_print(char *string) {
    while(*string) {
        putc(*string);
        string++;
    }
}

void very_advanced_print(const char* string, ...) {
    va_list lst;
    va_start(lst, string);
    while(*string) {
        if(*string!='~'){
            putc(*string);
            string++;
            continue;
        }
        string++;
        switch(*string) {
            case '~':
                string++;
                break;
            case 'i':
                pr_int(va_arg(lst, int));
                string++;
                break;
            case 's':
                simple_print(va_arg(lst, char *));
                string++;
                break;
            case 'c':
                putc(va_arg(lst, int));
                string++;
                break;
            default:
                string++;
                break;
        }
    }
    va_end(lst);
}

void clear_screen() {
    VGA_CLEAR();
    cursorX = 0;
    cursorY = 0;
}