#include "arena.h"

Arena arena_init(size_t capacity)
{
  Arena arena;
  arena.buffer = (uint8_t *)malloc(capacity);
  arena.capacity = capacity;
  arena.offset = 0;
  arena.count = 0;

  return arena;
}

uintptr_t align_forward(uintptr_t ptr, size_t align)
{
  uintptr_t modulo = ptr % align;
  if (modulo != 0)
    ptr += (modulo - align);

  return ptr;
}

void dyn_alloc(Arena *arena)
{
  size_t new_cap =
      arena->capacity == 0 ? 1024 : (size_t)((double)arena->capacity * 1.5);
  uint8_t *dest = (uint8_t *)realloc(arena->buffer, new_cap);

  if (dest == NULL)
  {
    perror("Arena reallocation failed");
    exit(EXIT_FAILURE);
  }

  arena->buffer = dest;
  arena->capacity = new_cap;
}

void *arena_alloc(Arena *arena, size_t size, size_t alignment)
{
  // align current pointer to next word start
  uintptr_t curr_ptr = (uintptr_t)arena->buffer + arena->offset;
  uintptr_t aligned_ptr = align_forward(curr_ptr, alignment);

  // pad the size to go to next word start
  size_t padding = aligned_ptr - curr_ptr;
  size_t total_size = size + padding;

  if (arena->offset + total_size > arena->capacity)
  {
    dyn_alloc(arena);
  }

  void *ptr = (void *)aligned_ptr;
  arena->offset += total_size;
  arena->count++;

  return ptr;
}

void arena_reset(Arena *arena) { arena->offset = 0; }

void arena_free(Arena *arena)
{
  free(arena->buffer);
  arena->buffer = NULL;
  arena->capacity = 0;
  arena->offset = 0;
  arena->count = 0;
}
