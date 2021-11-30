#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileio.h"
#include "lexer.h"

static void print_tokens(token* tokens, ulong count)
{
	printf("TOKENS:\n");

	char buffer[0x100];
	for (int i = 0; i < count; i++)
	{
		memset(buffer, '\0', ARRLEN(buffer));

		token* t = &tokens[i];
		const char* type_str = NULL;
		switch (t->type)
		{
		case IDENTIFIER:	type_str = "identifier";	break;
		case LITERAL:		type_str = "literal";		break;
		case LPAREN:		type_str = "lparen";		break;
		case RPAREN:		type_str = "rparen";		break;
		case OPERATOR:		type_str = "operator";		break;
		case LBRACE:		type_str = "lbrace";		break;
		case RBRACE:		type_str = "rbrace";		break;
		case SEMICOLON:		type_str = "semicolon";		break;
		}
		strncpy_s(buffer, ARRLEN(buffer), t->string, t->len);
		printf("%s\t%s\n", type_str, buffer);
	}
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		PRINT_ERR("provide a filepath");
		return 1;
	}
	const char* filepath = argv[1];

	char* src_code = NULL; ulong src_code_size = 0;
	error_code e = load_file(filepath, &src_code, &src_code_size);
	if (e)
	{
		PRINT_ERR("failed to read file");
		if (src_code) free(src_code);
		return 1;
	}

	token* tokens = NULL; ulong token_count = 0;
	// keep src_code alive, so tokens can cite it for identifiers
	e = lex_analyze(src_code, src_code_size, &tokens, &token_count);
	if (e)
	{
		PRINT_ERR("error while performing lexical analysis");
		if (tokens) free(tokens);
		free(src_code);
		return 1;
	}
	print_tokens(tokens, token_count);

	// now the src_code is no longer needed (make 100% sure all tokens are freed)
	free(tokens);
	free(src_code);

	return 0;
}
