#include "gfx/con.h"
#include "gfx/vga.h"
#include "theweirdstuff/idt.h"
#include "theweirdstuff/pit.h"
#include "input/read.h"
#include "memory/malloc.h"
#include "memory/mem.h"
#include "util/strutil.h"

extern "C" void kmain(void) {
    static uint16_t kcs = 0x08;
    init_interrupts(kcs);
    char* input = (char *)cmalloc(256);
    input[0] = (char)0;
    while(strncmp(input, "stop", 5)!=0) {
        read(input, 256, 1);
    }
    cfree(input);
}