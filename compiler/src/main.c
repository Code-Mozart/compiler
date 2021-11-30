#include <stdio.h>
#include <stdlib.h>

#include "fileio.h"
#include "lexer.h"


// todo: remove
#include <string.h>

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
		free(src_code);
		if (tokens) free(tokens);
		return 1;
	}

	//printf("TOKENS:\n");
	//char* chars = NULL;
	//for (int i = 0; i < token_count; i++)
	//{
	//	token* t = &tokens[i];
	//	const char* type_str = NULL;
	//	switch (t->type)
	//	{
	//	case IDENTIFIER:	type_str = MAKE_STRING(IDENTIFIER);	break;
	//	case LITERAL:		type_str = MAKE_STRING(LITERAL);	break;
	//	case LPAREN:		type_str = MAKE_STRING(LPAREN);		break;
	//	case RPAREN:		type_str = MAKE_STRING(RPAREN);		break;
	//	case OPERATOR:		type_str = MAKE_STRING(OPERATOR);	break;
	//	case LBRACE:		type_str = MAKE_STRING(LBRACE);		break;
	//	case RBRACE:		type_str = MAKE_STRING(RBRACE);		break;
	//	case SEMICOLON:		type_str = MAKE_STRING(SEMICOLON);	break;
	//	}
	//	chars = (char*)malloc(t->len + 1);
	//	memset(chars, '\0', t->len + 1);
	//	printf("%s\t%s\n", type_str, chars);
	//	free(chars);
	//}

	// now the src_code is no longer needed
	free(src_code);

	return 0;
}
