#ifndef AF_MEMORY_H
#define AF_MEMORY_H

#include <stddef.h>

void *af_malloc(size_t size);
void af_free(void *ptr);
void *af_calloc(size_t count, size_t size);
void *af_realloc(void *ptr, size_t size);

#endif