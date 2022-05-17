#pragma once

#include <stdio.h>

typedef unsigned short error_code;

#define PRINT_ERR(msg) printf("!! [%s:%d] %s\n", __FILE__, __LINE__, msg)
#define PRINTF_ERR(format, ...) printf("!! [%s:%d] "format"\n", __FILE__, __LINE__, __VA_ARGS__)

#define TRY(x) { error_code e = x; if (e) return e; }


#define SUCCESS					((error_code) 0x0000)		// success
#define EX_UNKNOWN				((error_code) 0x0001)		// unknown exception
#define EX_MALLOC				((error_code) 0x0001)		// failed to allocate memory

#define EX_ARG					((error_code) 0x0010)		// ARGUMENT EXCEPTION
#define EX_NULL					((error_code) 0x0011)		// argument must not be null
#define EX_INV_ARG				((error_code) 0x0012)		// invalid argument
#define EX_OUTARG_NOT_NULL		((error_code) 0x0013)		// output argument must be null

#define EX_ARR					((error_code) 0x0020)		// ARRAY EXCEPTION
#define EX_EMPTY				((error_code) 0x0021)		// can not remove from empty array
#define EX_FULL					((error_code) 0x0022)		// can not add to full array
#define EX_INDEX				((error_code) 0x0023)		// index out of bounds
#define EX_NOT_FOUND			((error_code) 0x0024)		// element not found in array

#define EX_FILE					((error_code) 0x0040)		// FILE EXCEPTION
