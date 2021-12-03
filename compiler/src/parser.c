#include "parser.h"

error_code build_ast(token* tokens, ulong token_count, ast_node** _ast_root)
{
	/*
	ast data layout considerations:
	> add padding for smaller nodes (+performance, -memory)
	>> separated arrays for each type/size of node (-complicated/verbose)
	> in-place allocation of child nodes (+memory, -performance)
	*/

	//...
}
