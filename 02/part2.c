#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_LINE_SIZE 500
char line[MAX_LINE_SIZE];
enum colors { RED, GREEN, BLUE };

char* parse_uint(char* start, char* end, uint64_t* value)
{
	char number_str[20];
	size_t len = end - start;
	memcpy(number_str, start, len);
	number_str[len] = '\0';
	*value = atoi(number_str);
	return end;
}

char* parse_color(char* start, enum colors* col)
{
	if (strncmp(start, "red", 3) == 0) {
		*col = RED;
		return start + 3;
	}

	if (strncmp(start, "green", 5) == 0) {
		*col = GREEN;
		return start + 5;
	}

	if (strncmp(start, "blue", 4) == 0) {
		*col = BLUE;
		return start + 4;
	}

	return start;
}

void parse_line(char* l, uint64_t* id, uint64_t* red, uint64_t* green, uint64_t* blue)
{
	*red = 0;
	*green = 0;
	*blue = 0;

	char* it = strchr(l, ' ') + 1;
	it = parse_uint(it, strchr(it, ':'), id);
	while (it && *it != '\n') {
		it += 2;
		// Parse number
		uint64_t value = 0;
		it = parse_uint(it, strchr(it, ' '), &value) + 1;
		
		// Parse color
		enum colors color;
		it = parse_color(it, &color);
		switch (color) {
			case RED:	if (value > *red) *red = value; break;
			case GREEN:	if (value > *green) *green = value; break;
			case BLUE:	if (value > *blue) *blue = value; break;
		}
	}
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
		uint64_t id, red, green, blue;
		parse_line(line, &id, &red, &green, &blue);
		count += red * green * blue;
	}

	fclose(input);
	printf("Result: %llu\n", count);

	return 0;
}
