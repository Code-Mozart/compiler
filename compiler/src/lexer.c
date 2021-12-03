#include "lexer.h"

#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "logger.h"

static char delimiters[] = {
	' ', '\n', '\t',
	';',
	'(', ')', '{', '}',
	'=', '+', '-', '<', '>'
};

// TOKEN LIST

static struct {
	token* buffer;
	ulong capacity;
	ulong count;
} token_list;

inline static error_code init_token_list(ulong initial_capacity)
{
	token_list.count = 0;
	token_list.buffer = (token*)calloc(initial_capacity, sizeof(token));
	if (!token_list.buffer)
	{
		token_list.capacity = 0;
		return EX_MALLOC;
	}
	token_list.capacity = initial_capacity;
	return SUCCESS;
}

inline static error_code resize_token_list(ulong min_capacity)
{
	// todo: remove
	printf("resizing token list\n");

	ulong new_capacity = min((token_list.capacity * 3L) / 2L, min_capacity);
	token* temp = (token*)calloc(new_capacity, sizeof(token));
	if (!temp) return EX_MALLOC;

	memcpy_s(temp, new_capacity, token_list.buffer, token_list.capacity);
	free(token_list.buffer);
	
	token_list.buffer = temp;
	token_list.capacity = new_capacity;
	return SUCCESS;
}

static error_code add_token(
	ulong line, ulong pos, enum token_type type, const char* const string, byte len, word value
)
{
	if (token_list.count >= token_list.capacity)
	{
		error_code e = resize_token_list(token_list.capacity + 1);
		if (e) return e;
	}

	token* t = &token_list.buffer[token_list.count++];
	t->line = line;
	t->pos = pos;
	t->type = type;
	t->string = string;
	t->len = len;
	t->value = value;
	return SUCCESS;
}

// LEXER

inline static bool is_int(char c) { return (c >= '0' && c <= '9'); }
inline static bool is_valid_char_1(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }
inline static bool is_valid_char(char c) { return is_valid_char_1(c) || is_int(c); }

error_code lex_analyze(const char* src, ulong size, token** _tokens, ulong* _count)
{
	if (!src || !_tokens || !_count) return EX_NULL;
	if (*_tokens) return EX_OUTARG_NOT_NULL;
	if (!size)
	{
		cm_log(WARNING, "src is empty");
		*_count = 0;
		return SUCCESS;
	}

	error_code e = init_token_list(size / 2);
	if (e) return e;

	// todo: add line & pos

	enum { comment = 0x01, skip_line = 0x02, delim_match = 0x04, int_literal = 0x08, negative = 0x10 };
	byte flags = 0x0;
	char* identifier_begin = NULL; byte identifier_len = 0;
	word value = 0;
	for (int i = 0; i < size; i++)
	{
		char c = src[i];
		if (c == '\r')
			continue;

		if (GET_BIT(flags, skip_line))
		{
			if (c == '\n')
				CLEAR_BIT(flags, skip_line);
			continue;
		}
		else if (GET_BIT(flags, comment))
		{
			if (src[i - 1] == '}' && c == '/')
				CLEAR_BIT(flags, comment);
			continue;
		}
		else if (c == '/')
		{
			if (i >= size - 1) return EX_EOF;
			else if (src[i + 1] == '/')
			{
				SET_BIT(flags, skip_line);
				i++;
				continue;
			}
			else if (src[i + 1] == '{')
			{
				SET_BIT(flags, comment);
				i++;
				continue;
			}
		}

		CLEAR_BIT(flags, delim_match);
		for (int j = 0; j < ARRLEN(delimiters); j++)
		{
			if (c == delimiters[j])
			{
				SET_BIT(flags, delim_match);
				break;
			}
		}
		if (GET_BIT(flags, delim_match))
		{
			if (identifier_begin)
			{
				e = add_token(0, 0, IDENTIFIER, identifier_begin, identifier_len, SHRT_MIN);
				if (e) return e;
				identifier_begin = NULL;
				identifier_len = 0;
			}
			else if (GET_BIT(flags, int_literal))
			{
				e = add_token(0, 0, LITERAL, NULL, 0, GET_BIT(flags, negative) ? -value : value);
				value = 0;
				CLEAR_BIT(flags, negative);
				CLEAR_BIT(flags, int_literal);
			}
			// todo: literals

			// todo: check special delims like '\n'
			enum token_type type;
			switch (c)
			{
			case ';':	type = SEMICOLON;	break;

			case '(':	type = LPAREN;		break;
			case ')':	type = RPAREN;		break;
			case '{':	type = LBRACE;		break;
			case '}':	type = RBRACE;		break;

			case '-':	SET_BIT(flags, negative);
			case '=':
			case '+':
			case '<':
			case '>':	type = OPERATOR;	break;

			default: continue;
			}
			e = add_token(0, 0, type, src + i, 1, SHRT_MIN);
			if (e) return e;
		}
		else
		{
			if (GET_BIT(flags, int_literal))
			{
				// todo: prevent overflow
				value = value * 10 + (c - '0');
			}
			else if (identifier_begin)
			{
				if (identifier_len >= UCHAR_MAX)
					return EX_MAX_IDENTIFIER_LEN;
				identifier_len++;
			}
			else
			{
				if (is_int(c))
				{
					value = (c - '0');
					SET_BIT(flags, int_literal);
				}
				else if (is_valid_char_1(c))
				{
					identifier_begin = src + i;
					identifier_len++;
				}
				else
				{
					return EX_ILLEGAL_CHAR;
				}
			}
		}
	}

	*_tokens = token_list.buffer;
	token_list.buffer = NULL;
	token_list.capacity = 0;
	*_count = token_list.count;
	token_list.count = 0;
	return SUCCESS;
}
