#include "ast.h"

#include "list.h"

/*
ast data layout considerations:
> add padding for smaller nodes (+performance, -memory)
>> separated arrays for each type/size of node (-complicated/verbose)
> in-place allocation of child nodes (+memory, -performance)
*/

#define INITIAL_CAPACITY 5

static list seqs_list;

static list decls_list;
static list assigns_list;
static list whiles_list;
static list calls_list;

static list consts_list;
static list vars_list;
static list bin_ops_list;

error_code init_ast()
{
	// todo: handle errors

	list_init(INITIAL_CAPACITY, sizeof(ast_sequence), &seqs_list);

	list_init(INITIAL_CAPACITY, sizeof(ast_decl), &decls_list);
	list_init(INITIAL_CAPACITY, sizeof(ast_assign), &assigns_list);
	list_init(INITIAL_CAPACITY, sizeof(ast_while), &whiles_list);
	list_init(INITIAL_CAPACITY, sizeof(ast_call), &calls_list);

	list_init(INITIAL_CAPACITY, sizeof(ast_const), &consts_list);
	list_init(INITIAL_CAPACITY, sizeof(ast_var), &vars_list);
	list_init(INITIAL_CAPACITY, sizeof(ast_bin_op), &bin_ops_list);

	return SUCCESS;
}

error_code free_ast()
{
	// todo: handle errors
	
	list_free(&seqs_list);

	list_free(&decls_list);
	list_free(&assigns_list);
	list_free(&whiles_list);
	list_free(&calls_list);

	list_free(&consts_list);
	list_free(&vars_list);
	list_free(&bin_ops_list);

	return SUCCESS;
}



// SEQUENCE

error_code ast_create_sequence(ast_sequence** _node, ulong line, ulong pos)
{
	if (!_node) return EX_NULL;
	if (*_node) return EX_OUTARG_NOT_NULL;

	error_code e = list_append(&seqs_list, sizeof(ast_sequence), _node);
	if (e) return e;
	(*_node)->node.type = AST_SEQUENCE;
	(*_node)->node.line = line;
	(*_node)->node.pos = pos;
	return SUCCESS;
}

error_code ast_seq_append(ast_sequence* node, ast_stm* stm)
{
	ast_stm** stm_ptr = NULL;
	error_code e = list_append(&(node->statements), sizeof(ast_stm*), &stm_ptr);
	if (e) return e;

	*stm_ptr = stm;
	return SUCCESS;
}



// DECLARATION

error_code ast_create_decl(ast_decl** _node, ulong line, ulong pos, const char* identifier, byte len)
{
	if (!_node) return EX_NULL;
	if (*_node) return EX_OUTARG_NOT_NULL;

	error_code e = list_append(&decls_list, sizeof(ast_decl), _node);
	if (e) return e;
	(*_node)->stm.node.type = AST_DECL;
	(*_node)->stm.node.line = line;
	(*_node)->stm.node.pos = pos;
	(*_node)->identifier = identifier; (*_node)->len = len;
	return SUCCESS;
}
