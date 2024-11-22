#include "arena.h"

Arena* newArena(u64 size)
{
    Arena* a = (Arena*)malloc(sizeof(Arena));
    memset(a, 0, sizeof(Arena));

    a->memory = (u8*)malloc(size);
    memset(a->memory, 0, size);

    a->ptr = a->memory;
    a->capacity = size;

    return a;
}

Arena* newArenaa(Arena* arena, u64 size)
{
    Arena* a = (Arena*)allocate(arena, size);
    memset(a, 0, sizeof(Arena));

    a->memory = (u8*)allocate(arena, size);
    memset(a->memory, 0, size);

    a->ptr = a->memory;
    a->capacity = size;

    return a;
}

void* allocate(Arena* a, u64 size)
{
    ASSERT(a->ptr + size < a->ptr + a->capacity);

    u8* ptr = a->ptr;
    a->ptr += size;
    a->usedCapacity += size;

    return ptr;
}

void* reallocate(Arena* a, void* ptr, u64 oldsize, u64 newsize)
{
    ASSERT(a->ptr + newsize < a->ptr + a->capacity);

    void* newptr = a->ptr;
    a->ptr += newsize;
    memcpy(newptr, ptr, oldsize);

    a->usedCapacity -= oldsize;
    a->usedCapacity += newsize;

    return newptr;
}

void freeArena(Arena* a)
{
    free(a->memory);
    free(a);
}

void clear(Arena* a)
{
    memset(a->memory, 0, a->capacity);
    a->ptr = a->memory;
    a->usedCapacity = 0;
}
