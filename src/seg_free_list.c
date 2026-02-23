#include "seg_free_list.h"

#define NUM_BUCKETS 12
#define MAX_RANGE 65536

static struct chunk_header *buckets_list[NUM_BUCKETS];

static size_t get_bucket_index(size_t size)
{
    size_t i = 0;
    size_t range = 32;
    while (1)
    {
        if (size <= range || range >= MAX_RANGE)
        {
            return i;
        }
        range *= 2;
        i++;
    }
}

static void remove_chunk(struct chunk_header *chunk)
{
    if (chunk == NULL)
    {
        return;
    }

    if (chunk->prev != NULL)
    {
        chunk->prev->next = chunk->next;
    }
    else
    {
        size_t index = get_bucket_index(get_size(chunk->size));
        buckets_list[index] = chunk->next;
    }

    if (chunk->next != NULL)
    {
        chunk->next->prev = chunk->prev;
    }

    chunk->next = NULL;
    chunk->prev = NULL;
}

// Find the right bucket and insert chunk in it
void add_chunk(struct chunk_header *chunk)
{
    if (chunk == NULL)
    {
        return;
    }

    size_t index = get_bucket_index(get_size(chunk->size));
    if (buckets_list[index] == NULL)
    {
        buckets_list[index] = chunk;
        chunk->next = NULL;
        chunk->prev = NULL;
        return;
    }

    chunk->next = buckets_list[index];
    chunk->prev = NULL;
    buckets_list[index]->prev = chunk;
    buckets_list[index] = chunk;
}

// Return a chunk for the given size range
struct chunk_header *get_chunk(size_t size)
{
    if (size == 0)
    {
        return NULL;
    }

    size_t index = get_bucket_index(size);
    struct chunk_header *chunk = buckets_list[index];
    if (chunk != NULL)
    {
        remove_chunk(buckets_list[index]);
    }

    return chunk;
}

