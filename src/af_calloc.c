#include "af_calloc.h"
#include "af_malloc.h"
#include <string.h>
#include <stdint.h>

void *af_calloc(size_t count, size_t size)
{
    if (count == 0 || size == 0)
    {
        return NULL;
    }

    if (count > SIZE_MAX / size)
    {
        return NULL;
    }

    size_t tot_size = count * size;

    void *ptr = af_malloc(tot_size);

    if (ptr == NULL)
    {
        return NULL;
    }

    memset(ptr, 0, tot_size);

    return ptr;
}
