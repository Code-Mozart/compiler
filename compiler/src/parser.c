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
	ast_create_sequence(&root, 0, 0);

	ast_decl* decl = NULL;
	ast_create_decl(&decl, 0, 0, "Hello world", 0);
	ast_seq_append(root, decl);

	return SUCCESS;
}
