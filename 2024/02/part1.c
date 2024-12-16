#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_LINE_SIZE 40
#define MAX_NUMBERS_PER_LINE 10

int sequence_is_safe(int* numbers, int size)
{
	if (numbers[0] == numbers[1])
		return 0;
	
	bool increasing = numbers[0] < numbers[1];
	int last_value = numbers[0];
	for (int i = 1; i < size; ++i) {
		if (increasing && (numbers[i] <= last_value || numbers[i] - last_value > 3))
			return 0;
		if (!increasing && (numbers[i] >= last_value || last_value - numbers[i] > 3))
			return 0;
		last_value = numbers[i];
	}

	return 1;
}

int main()
{
	FILE* input = fopen("input.txt", "r");
	if (input == NULL) {
		printf("Unable to open input.txt\n");
		exit(1);
	}

	int safe_sequencies = 0;
	char line[MAX_LINE_SIZE];
	int list[MAX_NUMBERS_PER_LINE];
	int size = 0;

	while (fgets(line, MAX_LINE_SIZE, input)) {
		char* start = NULL;
		char* offset = line;
		
		while (offset != start) {
			start = offset;
			int value = (int)strtol(start, &offset, 10);
			if (offset != start)
				list[size++] = value;
		}
		
		safe_sequencies += sequence_is_safe(list, size);
		size = 0;
	}

	printf("Safe sequencies: %d\n", safe_sequencies);
	return 0;
}
