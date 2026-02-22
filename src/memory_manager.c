#include "memory_manager.h"

#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

// Better compatibility between Linux and macOS
#ifndef MAP_ANON
#define MAP_ANON MAP_ANONYMOUS
#endif

static void add_boundaries(void *heap_head, size_t size)
{
    size_t *fake_foot = (size_t *)heap_head;
    *fake_foot = PACK(0, 0);

    size_t *fake_head = (size_t *)((char *)heap_head +  size - sizeof(size_t));
    *fake_head = PACK(0, 0);
}

struct chunk_header *request_new_heap(size_t size)
{
    const size_t PAGE_SIZE = sysconf(_SC_PAGESIZE);
    const size_t ALIGNED_SIZE = (size + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1);

    void *heap_head = mmap(NULL, ALIGNED_SIZE, PROT_READ | PROT_WRITE, MAP_ANON |
        MAP_PRIVATE,
        -1, 0);

    if (heap_head == MAP_FAILED)
    {
        perror("request_new_heap");
        return NULL;
    }

    add_boundaries(heap_head, ALIGNED_SIZE);

    struct chunk_header *chunk = (struct chunk_header *)((char *)heap_head + sizeof(size_t));

    size_t real_size = ALIGNED_SIZE - (2 * sizeof(size_t));
    chunk->size = PACK(real_size, 1);

    size_t *footer = GET_FOOTER(chunk);
    *footer = chunk->size;

    return chunk;
}