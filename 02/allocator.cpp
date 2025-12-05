#include "allocator.hpp"

void init_buffer(Allocator* alloc, size_t max_size) {
    alloc->buf = new char[max_size];
    alloc->max_size = max_size;
    alloc->offset = 0;
}

Allocator* init_allocator(size_t max_size) {
    Allocator* alloc = new Allocator();
    init_buffer(alloc, max_size);
    return alloc;
}

Allocator* init_allocator(Allocator* alloc, size_t max_size) {
    if (!alloc) {
        return init_allocator(max_size);
    }

    delete[] alloc->buf;
    init_buffer(alloc, max_size);
    return alloc;
}

char* alloc(Allocator* alloc, size_t size) {
    if (!alloc || !alloc->buf) {
        return nullptr;
    }

    if (alloc->offset + size > alloc->max_size) {
        return nullptr;
    }

    char* buf_offset = alloc->buf + alloc->offset;
    alloc->offset += size;
    return buf_offset;
}

void reset(Allocator* alloc) {
    if (!alloc) return;

    alloc->offset = 0;
}

void clear(Allocator* alloc) {
    if (!alloc) return;

    delete[] alloc->buf;
    alloc->buf = nullptr;
    delete alloc;
}