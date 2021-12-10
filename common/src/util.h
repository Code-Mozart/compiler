#pragma once

#include <stdbool.h>

#define ARRLEN(arr) (sizeof(arr) / sizeof(arr[0]))

#define GET_BIT(bits, mask) (bits & mask)
#define SET_BIT(bits, mask) bits |= mask
#define CLEAR_BIT(bits, mask) bits &= ~mask
#define FLIP_BIT(bits, mask) bits ^= mask

#define MAKE_STRING(x) #x
#define TO_STR_BUFFER(ptr, len, max)	char buffer[max];					\
										memset(buffer, '\0', max);			\
										strncpy_s(buffer, max, ptr, len)
#define PRINT_STR(ptr, len, max)	{									\
										TO_STR_BUFFER(ptr, len, max);	\
										printf("%s", buffer);			\
									}

typedef unsigned char byte;
typedef short int word;

typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long int ulong;
