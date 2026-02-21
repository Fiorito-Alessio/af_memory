#ifndef CHUNK_HEADER_H
#define CHUNK_HEADER_H

#include <stddef.h>

// Bitwise manipulation macros
#define PACK(size, is_free) ((size) | (is_free))
#define GET_SIZE(size)      ((size) & ~1)
#define IS_FREE(size)       ((size) & 1)

// Get its own footer for af_free to work properly
#define GET_FOOTER(header_ptr) \
((size_t *)((char *)(header_ptr) + GET_SIZE((header_ptr)->size) - sizeof(size_t)))

// Get prev chunk footer for af_free to work properly
#define GET_PREV_FOOTER(header_ptr) \
((size_t *)((char *)(header_ptr) - sizeof(size_t)))


/*
 * Contains all metadata for a memory chunk.
 * is_free / in_use bit is stored in size (multiple of 8)
 * ptr next / prev are used if block is free.
 * Otherwise, payload is used when block is in use.
 */
struct chunk_header
{
    size_t size;
    union
    {
        struct
        {
            struct chunk_header *next;
            struct chunk_header *prev;
        };
        char payload[0];
    };
};

#endif
