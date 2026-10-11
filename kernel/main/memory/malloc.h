#pragma once

#include <stddef.h>
#include <stdbool.h>

#define heap_size 25000

extern char heap[heap_size];

typedef struct memory {
    bool isFree;          // is this block free?
    size_t size;          // size of the payload (without header)
    struct memory *next;  // next block in the list
} memory;

extern memory *head;

void *cmalloc(size_t size);
void cfree(void *ptr);