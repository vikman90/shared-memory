#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <shm.h>

static unsigned int shm_hysteresis = 1;

void* shm_alloc(shm_t *mem, size_t size) {
    int pagesize = getpagesize();
    mem->length = ((size - 1) / pagesize + 1) * pagesize;
    mem->addr = mmap(NULL, mem->length, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (!mem->addr)
        return NULL;

    mem->size = size;
    return mem->addr;
}

void* shm_realloc(shm_t *mem, size_t size) {
    int pagesize = getpagesize();
    size_t length = ((size - 1) / pagesize + 1) * pagesize;

    if (length > mem->length || length + shm_hysteresis * pagesize < mem->length) {
        void *addr = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

        if (!addr)
            return NULL;

        memcpy(addr, mem->addr, size < mem->size ? size : mem->size);
        munmap(mem->addr, mem->length);

        mem->addr = addr;
        mem->length = length;
    }

    mem->size = size;
    return mem->addr;
}

void shm_free(shm_t *mem) {
    munmap(mem->addr, mem->length);
}

unsigned int shm_get_hysteresis() {
    return shm_hysteresis;
}

void shm_set_hysteresis(unsigned int value) {
    shm_hysteresis = value;
}
