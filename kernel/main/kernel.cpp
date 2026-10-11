#include "gfx/con.h"
#include "gfx/vga.h"

extern "C" void kmain(void) {
    for(;;) {
        for(int i = 0; i < 0x0F; i++) {
            SET_COLOUR(i);
            very_advanced_print("BMOS ON TOP!\n");
        }
    }
}