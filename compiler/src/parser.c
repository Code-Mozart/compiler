#include "parser.h"

#include "logger.h"

error_code build_ast(token* tokens, ulong token_count, ast_node** _ast_root)
{
	if (!tokens || !_ast_root) return EX_NULL;
	if (*_ast_root) return EX_OUTARG_NOT_NULL;
	if (!token_count)
		// src must have been empty
		return SUCCESS;

	error_code e = init_ast();
	if (e) return e;

	ast_sequence* root = NULL;
	ast_create_sequence(&root, 1, 1);

	*_ast_root = root;
	return SUCCESS;
}
