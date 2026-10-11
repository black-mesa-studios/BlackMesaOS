#pragma once
#include <stdint.h>

void VGA_CURSOR_MOVE(int X, int Y);
void VGA_WRITE(char ch, int X, int Y);
void VGA_CLEAR();
void SET_COLOUR(uint8_t clr);