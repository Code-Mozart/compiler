#include <stdio.h>
#include <stdlib.h>

#include "mystruct.h"
#include "creator.h"

int main(int argc, char** argv)
{
	mystruct* ptr; int count;
	create_ptr(&ptr, &count);
	free(ptr);

	return 0;
}
