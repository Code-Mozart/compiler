#include "creator.h"

void create_ptr(char** _ptr, int* _count)
{
	mystruct* ptr = (mystruct*)calloc(56 * sizeof(mystruct));
	int count = 30;

	*_ptr = ptr;
	*_count = count;
}
