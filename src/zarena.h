#ifndef ARENA_H
#define ARENA_H

#include "ztypes.h"

#define KILOBYTES(number) ((number) * 1024ull)
#define MEGABYTES(number) (KILOBYTES(number) * 1024ull)
#define GIGABYTES(number) (MEGABYTES(number) * 1024ull)
#define TERABYTES(number) (GIGABYTES(number) * 1024ull)

// clang-format off
// macos memory management: https://developer.apple.com/library/archive/documentation/Performance/Conceptual/ManagingMemory/Articles/AboutMemory.html
// Untangling Lifetimes - The Arena Allocator: https://www.rfleury.com/p/untangling-lifetimes-the-arena-allocator
// clang-format on

typedef struct {
    u8* memory;
    u8* ptr;
    u64 capacity;
} Arena;

Arena* newArena(u64 size);

Arena* newArenaa(Arena* arena, u64 size);

void* allocate(Arena* a, u64 size);

void* reallocate(Arena* a, void* ptr, u64 oldsize, u64 newsize);

void clear(Arena* a);

void freeArena(Arena* a);

#endif // ARENA_H
