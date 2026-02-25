#include "af_free.h"
#include "chunk_header.h"
#include "seg_free_list.h"

void af_free(void *ptr)
{
    if (ptr == NULL)
    {
        return;
    }

    pthread_mutex_lock(&malloc_mutex);

    struct chunk_header *head = get_header(ptr);
    size_t tot_size = get_size(head->size);

    struct chunk_header *right = (struct chunk_header *)((char *)head + get_size(head->size));
    size_t *left_foot = get_prev_footer(head);

    if (is_free(right->size))
    {
        tot_size += get_size(right->size);
        remove_chunk(right);
    }

    if (is_free(*left_foot))
    {
        struct chunk_header *left = (struct chunk_header *)((char *)head - get_size(*left_foot));
        tot_size += get_size(left->size);
        remove_chunk(left);
        head = left;
    }

    head->size = pack(tot_size, 1);
    size_t *new_foot = get_footer(head);
    *new_foot = head->size;

    add_chunk(head);

    pthread_mutex_unlock(&malloc_mutex);
}
