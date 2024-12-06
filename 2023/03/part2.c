#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_LINE_SIZE 500

const char digits[] = "0123456789";

uint64_t parse_uint(char* start, char* end)
{
	char number_str[20];
	size_t len = end - start;
	memcpy(number_str, start, len);
	number_str[len] = '\0';
	return atoi(number_str);
}

uint8_t is_digit(const char ch)
{
	return strchr(digits, ch) != NULL;
}

uint64_t process_gear(char* above, char* line, char* below, size_t offset)
{
	uint64_t numbers[2] = { 0, 0 };
	uint64_t number_count = 0;
	char* lines[3] = { above, line, below };

	uint8_t last_was_digit = 0;
	for (uint8_t i = 0; i < 9; ++i) {
		
		int64_t off = offset - 1 + i % 3;
		if (off < 0) continue;

		if (i % 3 == 0) 
			last_was_digit = 0;

		char* l = lines[i / 3];
		uint8_t ch_is_digit = is_digit(l[off]);

		if (ch_is_digit && !last_was_digit) {
			if (number_count == 2) return 0;

			char* it = l + off;
			while (it - l >= 0 && is_digit(*it)) --it;
			++it;
			numbers[number_count] = atoi(it);
			number_count++;
			last_was_digit = 1;
		}
		else if (!ch_is_digit) {
			last_was_digit = 0;
		}
	}

	return numbers[0] * numbers[1];
}

uint64_t process_line(char* above, char* line, char* below)
{
	uint64_t sum = 0;
	char* it = line;
	while (*it) {
		// Find next gear
		it = strchr(it, '*');
		if (!it) break;

		sum += process_gear(above, line, below, it - line);
		it++;
	}

	return sum;
}


int32_t main(int32_t argc, char* argv[]) {

	if (argc != 2) {
		printf("No input file\n");
		exit(1);
	}

	FILE* input = fopen(argv[1], "r");
	if (!input) {
		printf("Error opening file %s\n", argv[1]);
		exit(1);
	}

	char line0[MAX_LINE_SIZE];
	char line1[MAX_LINE_SIZE];
	char line2[MAX_LINE_SIZE];
	memset(line0, 0, MAX_LINE_SIZE);
	memset(line1, 0, MAX_LINE_SIZE);
	memset(line2, 0, MAX_LINE_SIZE);

	char* lines[] = { line0, line1, line2 };
	uint64_t line_value = 0;
	uint64_t count = 0;

	// Read first 2 lines and process the first
	fgets(line0, MAX_LINE_SIZE, input);
	fgets(line1, MAX_LINE_SIZE, input);
	count += process_line(NULL, line0, line1);

	// Process middle of document
	while (fgets(lines[2], MAX_LINE_SIZE, input)) {
		count += process_line(lines[0], lines[1], lines[2]);
		char* ptr = lines[0];
		lines[0] = lines[1];
		lines[1] = lines[2];
		lines[2] = ptr;
		memset(lines[2], 0, MAX_LINE_SIZE);
	}

	// Porcess last line
	count += process_line(lines[0], lines[1], NULL);

	fclose(input);
	printf("Result: %llu\n", count);

	return 0;
}
