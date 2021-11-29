#include <stdio.h>
#include <stdlib.h>

#include "fileio.h"

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("!! provide a filepath\n");
		return 1;
	}
	const char* filepath = argv[1];

	char* src_code = NULL;
	ulong src_code_size = 0;
	error_code e = load_file(filepath, &src_code, &src_code_size);
	if (e)
	{
		printf("!! failed to read file\n");
		if (src_code) free(src_code);
		return 1;
	}

	return 0;
}
