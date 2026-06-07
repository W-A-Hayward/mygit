#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
  uint8_t *buffer;
  size_t capacity;
  size_t offset;
} Arena;

Arena arena_init(size_t capacity);
static inline uintptr_t align_forward(uintptr_t ptr, size_t align);
void *arena_alloc(Arena *arena, size_t size, size_t alignment);
void arena_reset(Arena *arena);
void arena_free(Arena *arena);
