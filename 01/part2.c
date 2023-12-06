#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_LINE_SIZE 100

char line[MAX_LINE_SIZE];

// Lookup table
char starting_letters[] = "otfsen";
char* keys[] = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

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
		int32_t last_ch = -1;
		for (char* it = line; it && *it != '\n'; ++it) {
			int32_t found_ch = -1;
			if ('0' <= *it && *it <= '9') {
				found_ch = *it - '0';
			}
			else if (strchr(starting_letters, *it)) {
				for (size_t x = 0; x < 9; ++x) {
					if (strstr(it, keys[x]) == it) {
						found_ch = x + 1;
						break;
					}
				}
			}
			
			if (found_ch != -1) {
				if (last_ch == -1)
					count += found_ch * 10;
				last_ch = found_ch;
			}
		}
		if (last_ch == -1)
			printf("%s", line);
		count += last_ch;
	}

	fclose(input);
	printf("Result: %llu\n", count);

	return 0;
}
