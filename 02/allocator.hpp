#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP

#include <cstddef>

struct Allocator {
    char* buf;
    size_t max_size;
    size_t offset;
};

Allocator* init_allocator(size_t max_size);
Allocator* init_allocator(Allocator* alloc, size_t max_size);
char* alloc(Allocator* alloc, size_t size);
void reset(Allocator* alloc);
void clear(Allocator* alloc);

#endif
