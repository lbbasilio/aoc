#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_SIZE 10000

int main()
{
	FILE* input = fopen("input.txt", "r");
	if (input == NULL) {
		printf("Unable to open input.txt\n");
		exit(1);
	}

	long sum = 0;
	long operand1;
	long operand2;
	char line[MAX_LINE_SIZE];

	while (fgets(line, MAX_LINE_SIZE, input)) {

		char* c = line;
		while (*c) {

			char* offset = strstr(c, "mul(");
			if (offset == NULL)
				break;

			c = offset + 4;
			operand1 = strtol(c, &offset, 10);
			if (offset - c == 0 || offset - c > 3) {
				c = offset; 
				continue;
			}

			c = offset;
			if (*c != ',')
				continue;
			c++;

			operand2 = strtol(c, &offset, 10);
			if (offset - c == 0 || offset - c > 3) {
				c = offset;
				continue;
			}

			c = offset;
			if (*c != ')')
				continue;
			c++;
			
			sum += operand1 * operand2;
		}
	}

	printf("Sum: %ld\n", sum);
	return 0;
}
