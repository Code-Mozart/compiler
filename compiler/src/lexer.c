#include "lexer.h"

#include <stdlib.h>

#include "logger.h"

static char delims[] = {
	' ', '\n', '\t',
	';', ',',
	'(', ')', '{', '}',
	'=', '+', '-',
	'<'
};

// TOKEN LIST

static struct {
	token* buffer;
	ulong len;
	ulong count;
} token_list;

inline static error_code init_token_list(ulong initial_capacity)
{
	token_list.count = 0;
	token_list.buffer = (token*)malloc(initial_capacity);
	if (!token_list.buffer)
	{
		token_list.len = 0;
		return EX_MALLOC;
	}
	token_list.len = initial_capacity;
	return SUCCESS;
}

// LEXER

error_code lex_analyze(char* src, ulong size, token** _tokens, ulong* _count)
{
	if (!src || !_tokens || !_count) return EX_NULL;
	if (*_tokens) return EX_OUTARG_NOT_NULL;
	if (!size)
	{
		log(WARNING, "src is empty");
		*_count = 0;
		return SUCCESS;
	}

	error_code e = init_token_list(size / 2);
	if (e)
	{
		free(src);
		return e;
	}

	//...

	free(src);
	return SUCCESS;
}
