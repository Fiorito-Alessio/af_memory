#ifndef SPLITTER_H
#define SPLITTER_H

#include "chunk_header.h"

struct chunk_header *split_chunk(struct chunk_header *big_chunk, size_t size);

#endif
