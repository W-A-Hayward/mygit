#include "arena.h"

Arena arena_init(size_t capacity)
{
  Arena arena;
  arena.buffer = (uint8_t *)malloc(capacity);
  arena.capacity = capacity;
  arena.offset = 0;
  return arena;
}

static inline uintptr_t align_forward(uintptr_t ptr, size_t align)
{
  uintptr_t modulo = ptr % align;
  if (modulo != 0)
    ptr += (modulo - align);

  return ptr;
}

void *arena_alloc(Arena *arena, size_t size, size_t alignment) {}
