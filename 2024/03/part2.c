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

	int should_add = 1;
	long sum = 0;
	long operand1;
	long operand2;
	char line[MAX_LINE_SIZE];

	while (fgets(line, MAX_LINE_SIZE, input)) {

		char* c = line;

		char* do_off = strstr(c, "do()");
		char* dont_off = strstr(c, "don't()");
		char* mul_off = strstr(c, "mul(");

		while (mul_off) {

			if (do_off && (!dont_off || do_off < dont_off) && do_off < mul_off) {
				should_add = 1;
				c = do_off + 4;
				do_off = strstr(c, "do()");
			}
			else if (dont_off && (!do_off || dont_off < do_off) && dont_off < mul_off) {
				should_add = 0;
				c = dont_off + 7;
				dont_off = strstr(c, "don't()");
			}
			else {

				c = mul_off + 4;
				if (should_add) {

					char* offset;
					operand1 = strtol(c, &offset, 10);
					if (offset - c == 0 || offset - c > 3) {
						c = offset; 
						goto MUL_ERR;
					}

					c = offset;
					if (*c != ',')
						goto MUL_ERR;
					c++;

					operand2 = strtol(c, &offset, 10);
					if (offset - c == 0 || offset - c > 3) {
						c = offset;
						goto MUL_ERR;
					}

					c = offset;
					if (*c != ')')
						goto MUL_ERR;
					c++;
					
					printf("mul\n");
					sum += operand1 * operand2;
				}
MUL_ERR:
				mul_off = strstr(c, "mul(");

			}
		}
	}

	printf("Sum: %ld\n", sum);
	return 0;
}
