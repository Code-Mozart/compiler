#include <stdio.h>
#include <stdlib.h>

#include "loader.h"
#include "processor.h"

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("!! provide a filepath\n");
		return 1;
	}
	const char* const filepath = argv[1];

	instruction* instructions = NULL;
	ulong count = 0;
	error_code e = load_bytecode(filepath, &instructions, &count);
	if (e)
	{
		if (e & EX_FILE)
			printf("!! failed to open file\n");
		else if (e == EX_UNKNOWN_OPCODE)
			printf("!! unknown instruction '%s' at line %d\n", loader_except.opcode, loader_except.line);
		else if (e & EX_LOAD_INSTRUCTIONS)
			printf("!! failed to read instructions\n");

		return 1;
	}

	run(instructions, count);
	return 0;
}
