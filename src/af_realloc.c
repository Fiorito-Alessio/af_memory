#include "af_realloc.h"
#include "af_malloc.h"
#include "af_free.h"
#include "chunk_header.h"

#include <string.h>
#include <stddef.h>

void *af_realloc(void *ptr, size_t size)
{
    if (ptr == NULL)
    {
        return af_malloc(size);
    }

    if (size == 0)
    {
        af_free(ptr);
        return NULL;
    }

    struct chunk_header *header = get_header(ptr);

    size_t tot_size = get_size(header->size);
    size_t payload_size = tot_size - (2 * sizeof(size_t));

    if (payload_size >= size)
    {
        return ptr;
    }

    void *new_ptr = af_malloc(size);

    if (new_ptr == NULL)
    {
        return NULL;
    }

    memcpy(new_ptr, ptr, payload_size);

    af_free(ptr);

    return new_ptr;
}