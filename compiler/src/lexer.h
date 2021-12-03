#pragma once

#include "util.h"
#include "except.h"
#include "token.h"

#define EX_LEXER				((error_code) 0x8000)	// LEXER EXCEPTION
#define EX_MAX_IDENTIFIER_LEN	((error_code) 0x8001)	// identifier length greater than maximum allowed
#define EX_EOF					((error_code) 0x8002)	// unexpected EOF (end of file), illegal last char
#define EX_ILLEGAL_CHAR			((error_code) 0x8003)	// illegal char, this char has no syntactic meaning

error_code lex_analyze(const char* src, ulong size, token** _tokens, ulong* _count);
