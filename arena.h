#ifndef ARENA_H_
#define ARENA_H_

#include <stdlib.h>

#define ARENA_OK			0
#define ARENA_INVALID_ARG	1
#define ARENA_ALLOC_FAILED	2

typedef struct {
	unsigned char* base;
	unsigned char* head;
	size_t capacity;
} Arena;

int arena_init(size_t capacity, Arena* arena);
int arena_alloc(Arena* arena, size_t bytes, void** ptr);
int arena_checkpoint(Arena* arena, unsigned char** checkpoint);
int arena_rollback(Arena* arena, unsigned char* ptr);
int arena_destroy(Arena* arena);

#endif

#ifdef ARENA_IMPLEMENTATION

int arena_init(size_t capacity, Arena* arena)
{
	if (arena == NULL)
		return ARENA_INVALID_ARG;
	
	void* data = malloc(capacity);
	if (data == NULL) {
		arena->base = NULL;
		arena->head = NULL;
		arena->capacity = 0;
		return ARENA_ALLOC_FAILED;
	}

	arena->base = data;
	arena->head = data;
	arena->capacity = capacity;
	return ARENA_OK;
}

int arena_alloc(Arena* arena, size_t bytes, void** ptr)
{
	if (arena == NULL || arena->base == NULL || bytes == 0 || ptr == NULL)
		return ARENA_INVALID_ARG;

	if (bytes > arena->capacity - (arena->head - arena->base))
		return ARENA_ALLOC_FAILED;

	*ptr = arena->head;
	arena->head += bytes;
	return ARENA_OK;
}

int arena_checkpoint(Arena* arena, unsigned char** checkpoint)
{
	if (arena == NULL || checkpoint == NULL)
		return ARENA_INVALID_ARG;

	*checkpoint = arena->head;
	return ARENA_OK;
}

int arena_rollback(Arena* arena, unsigned char* checkpoint)
{
	if (arena == NULL || checkpoint < arena->base || checkpoint >= arena->base + arena->capacity)
		return ARENA_INVALID_ARG;

	arena->head = checkpoint;
	return ARENA_OK;
}

int arena_destroy(Arena* arena) 
{
	if (arena == NULL)
		return ARENA_INVALID_ARG;

	free(arena->base);
	return ARENA_OK;
}

#undef ARENA_IMPLEMENTATION
#endif
