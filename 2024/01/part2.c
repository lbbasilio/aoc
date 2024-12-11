#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_SIZE 40
#define MAX_NUMBERS_SIZE 1000

#define ARENA_IMPLEMENTATION
#include "../../arena.h"

#define HASH_MAP_IMPLEMENTATION
#include "../../hash_map.h"

int comp(const void* a, const void* b)
{
	return *(int*)a - *(int*)b;
}

int abs(int n)
{
	return (n > 0) * n + !(n < 0) * (-n);
}

int main()
{
	FILE* input = fopen("input.txt", "r");
	if (input == NULL) {
		printf("Unable to open input.txt\n");
		exit(1);
	}

	char line[MAX_LINE_SIZE];
	int list1[MAX_NUMBERS_SIZE];
	int list2[MAX_NUMBERS_SIZE];
	int size = 0;

	while (fgets(line, MAX_LINE_SIZE, input)) {
		char* offset;
		list1[size] = (int)strtol(line, &offset, 10);
		list2[size] = (int)strtol(offset + 1, NULL, 10);
		size++;
	}

	Arena arena;
	arena_init(0x10000, &arena);

	Hash_Map map;
	hash_map_init(&map, &arena);

	for (int i = 0; i < size; ++i) {
		int value = 0;
		hash_map_get(&map, list2[i], &value);
		value++;
		hash_map_set(&map, list2[i], value);
	}

	long similarity = 0;	
	for (int i = 0; i < size; ++i) {
		int value = 0;
		hash_map_get(&map, list1[i], &value);
		similarity += value * list1[i];		
	}

	printf("Result: %lld\n", similarity);
	return 0;
}
