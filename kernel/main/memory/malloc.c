#include "malloc.h"
#include <stddef.h>
#include <stdint.h>

/* MIT License

Copyright (c) 2023 Milinda Shehan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#define ALIGNMENT   _Alignof(max_align_t)
// header size rounded up so the payload that follows it is always aligned
#define HEADER_SIZE ((sizeof(memory) + ALIGNMENT - 1) & ~(ALIGNMENT - 1))

_Alignas(max_align_t) char heap[heap_size];
memory *head = NULL;

static size_t align_up(size_t size)
{
    if (size > SIZE_MAX - (ALIGNMENT - 1))
        return 0; // Overflow

    return (size + ALIGNMENT - 1) & ~(ALIGNMENT - 1);
}

void *cmalloc(size_t size)
{
    if (size == 0)
        return NULL;

    size = align_up(size);
    if (size == 0)
        return NULL; // overflow while aligning

    // create first memory block
    if (head == NULL)
    {
        head = (memory *)&heap[0];
        head->size = (heap_size - HEADER_SIZE) & ~(ALIGNMENT - 1);
        head->isFree = true;
        head->next = NULL;
    }

    // find first free block that is large enough
    memory *current = head;
    while (current != NULL && !(current->isFree && current->size >= size))
    {
        current = current->next;
    }

    if (current == NULL)
        return NULL; // out of memory

    // split the block if the remainder is big enough to be useful
    if (current->size - size >= HEADER_SIZE + ALIGNMENT)
    {
        memory *newBlock = (memory *)((char *)current + HEADER_SIZE + size);
        newBlock->size = current->size - size - HEADER_SIZE;
        newBlock->isFree = true;
        newBlock->next = current->next;

        current->size = size;
        current->next = newBlock;
    }

    current->isFree = false;
    return (void *)((char *)current + HEADER_SIZE);
}

void cfree(void *ptr)
{
    if (ptr == NULL)
        return;

    memory *current = head;
    memory *previous = NULL;

    // find the block to be freed
    while (current != NULL && (char *)current + HEADER_SIZE != (char *)ptr)
    {
        previous = current;
        current = current->next;
    }

    if (current == NULL)
        return; // invalid pointer

    if (current->isFree)
        return; // double free

    current->isFree = true;

    // merge with next block first
    if (current->next != NULL && current->next->isFree)
    {
        current->size += current->next->size + HEADER_SIZE;
        current->next = current->next->next;
    }

    // then merge with previous block
    if (previous != NULL && previous->isFree)
    {
        previous->size += current->size + HEADER_SIZE;
        previous->next = current->next;
    }
}