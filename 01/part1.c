#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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
	int32_t c, last_ch = -1;
	while ((c = fgetc(input)) != EOF) {
		if ('0' <= c && c <= '9') {
			if (last_ch == -1) {
				count += (c - '0') * 10;
			}
			last_ch = c - '0';
		}
		else if (c == '\n') {
			count += last_ch;
			last_ch = -1;
		}
	}

	fclose(input);
	printf("Result: %llu\n", count);

	return 0;
}
