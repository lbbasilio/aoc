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

uint8_t is_symbol(const char ch)
{
	return ch != '\n' && strchr(".0123456789", ch) == NULL;
}

uint64_t process_line(char* above, char* line, char* below)
{
	uint64_t sum = 0;
	char* it = line;
	while (*it) {
		// Find beginning of next number
		size_t len = strcspn(it, digits);
		it += len;

		// Find number length
		len = strspn(it, digits);
		if (len) {
			size_t offset = it - line;
			uint8_t has_symbol = 0;

			// Check on same line
			if (it > line) has_symbol |= is_symbol(*(it - 1));
			has_symbol |= is_symbol(it[len]);

			// Check on lines above and below
			for (size_t i = 0; i < len + 2; ++i) {
				size_t index = offset - 1 + i;
				if (index < 0) continue;
				if (above) has_symbol |= is_symbol(above[index]);
				if (below) has_symbol |= is_symbol(below[index]);
			}

			if (has_symbol) sum += parse_uint(it, it + len);
		}
		it += len;
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
