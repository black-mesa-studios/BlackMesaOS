#include "vga.h"
#include <stdint.h>
#include "../port/io.h"

volatile uint8_t *vga = (volatile uint8_t*)0xB8000;
static uint8_t colour = 0x07;

void VGA_WRITE(char ch, int X, int Y) {
    if (X < 0 || X >= 80 || Y < 0 || Y >= 25)
        return;
    uint32_t offset = (Y * 80 + X) * 2;
    vga[offset] = (uint8_t)ch;
    vga[offset + 1] = colour;
}

void VGA_CLEAR() {
    for(int i = 0; i < 80; i++) {
        for(int j = 0; j < 25; j++) {
            VGA_WRITE(' ', i, j);
        }
    }
}

const uint8_t VGA_DEFAULT_COLOUR = 0x07;

void SET_COLOUR(uint8_t clr) {
    colour = clr;
}

void VGA_CURSOR_MOVE(int X, int Y) {
    if (X >= 80) {
        X = 0;
        Y++;
    } if (Y >= 25) {
        Y = 24;
    }
    uint16_t position = Y * 80 + X;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t) (position & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t) ((position >> 8) & 0xFF));
}