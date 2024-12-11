#ifndef HASH_MAP_H_
#define HASH_MAP_H_

#ifndef HASH_MAP_BUCKET_SIZE
#define HASH_MAP_BUCKET_SIZE 17
#endif

#include "arena.h"

#define HASH_MAP_OK				0
#define HASH_MAP_INVALID_ARG	1
#define HASH_MAP_ALLOC_FAILED	2
#define HASH_MAP_NOT_FOUND		3

struct Hash_Map_Node {
	struct Hash_Map_Node* next;
	int key;
	int value;
};

typedef struct {
	struct Hash_Map_Node* buckets[HASH_MAP_BUCKET_SIZE];
	Arena* arena;
} Hash_Map;

int hash_map_init(Hash_Map* map, Arena* arena);
int hash_map_set(Hash_Map* map, int key, int value);
int hash_map_get(Hash_Map* map, int key, int* value);

#endif

#ifdef HASH_MAP_IMPLEMENTATION

static size_t hash_map_hash_(int key)
{
	size_t hash = 0;
	char* ptr = (char*)&key;
	for (int i = 0; i < sizeof(int); ++i)
		hash = hash * 31 + *(ptr++);

	return hash % HASH_MAP_BUCKET_SIZE;
}

int hash_map_init(Hash_Map* map, Arena* arena)
{
	if (map == NULL || arena == NULL)
		return HASH_MAP_INVALID_ARG;
	
	map->arena = arena;
	memset(map->buckets, 0, sizeof(struct Hash_Map_Node*) * HASH_MAP_BUCKET_SIZE);
	return HASH_MAP_OK;
}

int hash_map_set(Hash_Map* map, int key, int value)
{
	if (map == NULL)
		return HASH_MAP_INVALID_ARG;
	
	size_t index = hash_map_hash_(key);
	struct Hash_Map_Node* last = NULL;
	struct Hash_Map_Node* node = map->buckets[index];

	while (node) {
		if (node->key == key) {
			// Key found, replace value
			node->value = value;
			return HASH_MAP_OK;
		}
		last = node;
		node = node->next;
	}

	int code = arena_alloc(map->arena,
						   sizeof(struct Hash_Map_Node),
						   (void**)&node);
	if (code != ARENA_OK)
		return HASH_MAP_ALLOC_FAILED;

	node->next = NULL;
	node->key = key;
	node->value = value;

	if (last) last->next = node;
	else map->buckets[index] = node;

	return HASH_MAP_OK;
}

int hash_map_get(Hash_Map* map, int key, int* value)
{
	if (map == NULL || value == NULL)
		return HASH_MAP_INVALID_ARG;

	size_t index = hash_map_hash_(key);
	struct Hash_Map_Node* node = map->buckets[index];

	while (node) {
		if (node->key == key) {
			*value = node->value;
			return HASH_MAP_OK;
		}
		node = node->next;
	}

	return HASH_MAP_NOT_FOUND;
}

#undef HASH_MAP_IMPLEMENTATION
#endif
