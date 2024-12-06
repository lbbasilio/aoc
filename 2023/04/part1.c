#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_NUM_COUNT 30
#define MAX_LINE_SIZE 500
char line[MAX_LINE_SIZE];

uint8_t is_digit(const char ch)
{
	return strchr("0123456789", ch) != NULL;
}

uint64_t process_line(char* line) 
{
	uint8_t winning[MAX_NUM_COUNT];
	uint8_t guesses[MAX_NUM_COUNT];

	uint64_t w_count = 0;
	uint64_t g_count = 0;

	char* it = strchr(line, ':') + 1;
	while (*it != '|') {
		if (is_digit(*it)) {
			winning[w_count] = atoi(it);
			w_count++;
			it = strchr(it, ' ');
		}
		else
			it++;
	}

	it++;
	while (it && *it) {
		if (is_digit(*it)) {
			guesses[g_count] = atoi(it);
			g_count++;
			it = strchr(it, ' ');
		}
		else
			it++;
	}

	uint64_t value = 0;
	for (uint8_t i = 0; i < w_count; ++i) {
		for(uint8_t j = 0; j < g_count; ++j) {
			if (winning[i] == guesses[j]) {
				if (!value) value = 1;
				else value <<= 1;
			}
		}
	}

	return value;
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

	uint64_t count = 0;
	while (fgets(line, sizeof(line), input)) {
		count += process_line(line);
	}

	fclose(input);
	printf("Result: %llu\n", count);

	return 0;
}
