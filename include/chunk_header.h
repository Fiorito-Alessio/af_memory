#ifndef CHUNK_HEADER_H
#define CHUNK_HEADER_H

#include <stddef.h>

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

static inline size_t pack(size_t size, size_t is_free)
{
    return (size | is_free);
}

static inline size_t get_size(size_t header_size)
{
    return (header_size & ~1UL);
}

static inline size_t is_free(size_t header_size)
{
    return (header_size & 1UL);
}

// Get its own footer for af_free to work properly
static inline size_t *get_footer(struct chunk_header *head_ptr)
{
    return ((size_t *)((char *)(head_ptr) + get_size((head_ptr)->size) - sizeof(size_t)));
}

// Get prev chunk footer for af_free to work properly
static inline size_t *get_prev_footer(struct chunk_header *head_ptr)
{
    return ((size_t *)((char *)(head_ptr) - sizeof(size_t)));
}

static inline struct chunk_header *get_header(void *ptr)
{
    return (struct chunk_header *)((char *)ptr - sizeof(size_t));
}

#endif
