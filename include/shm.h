#ifndef SHM_H
#define SHM_H

#include <stddef.h>

typedef struct shm_t {
    void *addr;
    size_t size;
    size_t length;
} shm_t;

void* shm_alloc(shm_t *mem, size_t size);
void* shm_realloc(shm_t *mem, size_t size);
void shm_free(shm_t *mem);

unsigned int shm_get_hysteresis();
void shm_set_hysteresis(unsigned int value);

#endif // SHM_H
