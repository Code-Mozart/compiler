#pragma once

#include "util.h"

enum token_type { KEYWORD, SYMBOL, SEMICOLON, IDENTIFIER, LITERAL };

typedef struct token
{
	ulong line, pos;
	enum token_type type;
	const char* const string;
	word value;
} token;
