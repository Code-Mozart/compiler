#pragma once

#include "util.h"

#define STR_CMP1(str, len, const_str) strcmp_len(str, len, const_str, sizeof(const_str) / sizeof(char) - 1)

bool strcmp_len(const char* const str1, const size_t len1, const char* const str2, const size_t len2);

byte uint_strlen(double value);
byte int_strlen(double value);

#ifdef IMPL_COMMON

#include <string.h>
#include <math.h>

bool strcmp_len(const char* const str1, const size_t len1, const char* const str2, const size_t len2)
{
	if (len1 != len2) return false;
	return memcmp(str1, str2, len1) == 0;
};

byte uint_strlen(double value)
{
	return ((byte)log10(value)) + 1;
};

byte int_strlen(double value)
{
	return uint_strlen(value) + (value < 0);
};

#endif
