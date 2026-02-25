#include "af_malloc.h"
#include "chunk_header.h"
#include "seg_free_list.h"
#include "memory_manager.h"
#include "splitter.h"
#include <pthread.h>

#define HEAP_REQ_SIZE (4 * 1024 * 1024)

pthread_mutex_t malloc_mutex = PTHREAD_MUTEX_INITIALIZER;

void *af_malloc(size_t size)
{
    if (size == 0)
    {
        return NULL;
    }

    pthread_mutex_lock(&malloc_mutex);

    size_t tot_size = size + (2 * sizeof(size_t));
    size_t align_16_size = (tot_size + (size_t)15) & (size_t)~15;

    struct chunk_header *chunk = get_chunk(align_16_size);
    if (chunk == NULL)
    {
        chunk = request_new_heap(HEAP_REQ_SIZE);
        if (chunk == NULL)
        {
            pthread_mutex_unlock(&malloc_mutex);
            return NULL;
        }
    }

    chunk = split_chunk(chunk, align_16_size);

    pthread_mutex_unlock(&malloc_mutex);
    return chunk->payload;
}
