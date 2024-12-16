#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_LINE_SIZE 40
#define MAX_NUMBERS_PER_LINE 10

int sequence_is_safe(int* numbers, int size, int ignore)
{
	int inc_count = 0;
	inc_count += numbers[0] < numbers[1];
	inc_count += numbers[1] < numbers[2];
	inc_count += numbers[2] < numbers[3];
	int dec_count = 0;
	dec_count += numbers[0] > numbers[1];
	dec_count += numbers[1] > numbers[2];
	dec_count += numbers[2] > numbers[3];

	if (inc_count == dec_count)
		return 0;

	bool increasing = inc_count > dec_count;
	int start_index = ignore == 0 ? 2 : 1;
	int last_value = numbers[start_index - 1];

	for (int i = start_index; i < size; ++i) {
		if (i == ignore)
			continue;

		if ((increasing && (numbers[i] <= last_value || numbers[i] - last_value > 3)) || 
			(!increasing && (numbers[i] >= last_value || last_value - numbers[i] > 3))) {
			
			if (ignore >= 0)
				return 0;

			return sequence_is_safe(numbers, size, i) ||
				   sequence_is_safe(numbers, size, i - 1);
		}

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
		
		safe_sequencies += sequence_is_safe(list, size, -1);
		size = 0;
	}

	printf("Safe sequencies: %d\n", safe_sequencies);
	return 0;
}
