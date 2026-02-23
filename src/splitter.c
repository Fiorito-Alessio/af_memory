#include "splitter.h"
#include "seg_free_list.h"

#define LEFT_MIN_SIZE 32

// size param is already perfectly CPU aligned and big enough for user's need
struct chunk_header *split_chunk(struct chunk_header *big_chunk, size_t size)
{
    struct chunk_header *req_chunk = big_chunk;

    size_t leftover = get_size(big_chunk->size) - size;

    if (leftover < LEFT_MIN_SIZE)
    {
        req_chunk->size = pack(get_size(req_chunk->size), 0);

        size_t *req_foot = get_footer(req_chunk);
        *req_foot = req_chunk->size;

        return req_chunk;
    }

    big_chunk = (struct chunk_header*)((char *)big_chunk + size);
    big_chunk->size = pack(leftover, 1);
    size_t *big_foot = get_footer(big_chunk);
    *big_foot = big_chunk->size;

    add_chunk(big_chunk);

    req_chunk->size = pack(size, 0);
    size_t *req_foot = get_footer(req_chunk);
    *req_foot = req_chunk->size;

    return req_chunk;
}