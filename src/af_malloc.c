#include "af_malloc.h"
#include "chunk_header.h"
#include "seg_free_list.h"
#include "memory_manager.h"
#include "splitter.h"

#define HEAP_REQ_SIZE (4 * 1024 * 1024)

void *af_malloc(size_t size)
{
    if (size == 0)
    {
        return NULL;
    }

    size_t tot_size = size + (2 * sizeof(size_t));
    size_t align_16_size = (tot_size + 15) & ~15;

    struct chunk_header *chunk = get_chunk(align_16_size);
    if (chunk == NULL)
    {
        chunk = request_new_heap(HEAP_REQ_SIZE);
        if (chunk == NULL)
        {
            return NULL;
        }
    }

    chunk = split_chunk(chunk, align_16_size);

    return chunk->payload;
}