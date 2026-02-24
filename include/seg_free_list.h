#ifndef SEG_FREE_LIST_H
#define SEG_FREE_LIST_H

#include "chunk_header.h"

void add_chunk(struct chunk_header *chunk);
struct chunk_header *get_chunk(size_t size);
void remove_chunk(struct chunk_header *chunk);

#endif
