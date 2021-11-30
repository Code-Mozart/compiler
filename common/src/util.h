#pragma once

#include <stdbool.h>

#define ARRLEN(arr) (sizeof(arr) / sizeof(arr[0]))
#define MAX_VAL_INT(var) (sizeof(var) * 0xFF)

#define GET_BIT(bits, mask) (bits & mask)
#define SET_BIT(bits, mask) bits |= mask
#define CLEAR_BIT(bits, mask) bits &= ~mask
#define FLIP_BIT(bits, mask) bits ^= mask

#define MAKE_STRING(x) #x

typedef unsigned char byte;
typedef short int word;

typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long int ulong;
