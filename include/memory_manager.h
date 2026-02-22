#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "chunk_header.h"

struct chunk_header *request_new_heap(size_t size);
#endif
