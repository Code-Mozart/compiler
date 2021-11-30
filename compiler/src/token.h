#pragma once

#include "util.h"

enum token_type {
	IDENTIFIER,
	LITERAL,
	LPAREN, RPAREN, OPERATOR,
	LBRACE, RBRACE, SEMICOLON
};

typedef struct token
{
	ulong line, pos;
	enum token_type type;
	const char* string;
	byte len;
	word value;
} token;
