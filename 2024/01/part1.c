#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_SIZE 40
#define MAX_NUMBERS_SIZE 1000

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

	qsort(list1, size, sizeof(int), comp);
	qsort(list2, size, sizeof(int), comp);

	long sum = 0;
	for (int i = 0; i < size; ++i)
		sum += abs(list1[i] - list2[i]);

	printf("Result: %lld\n", sum);
	return 0;
}
