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

	list_init(INITIAL_CAPACITY,	sizeof(ast_sequence),	&seqs_list);

	list_init(INITIAL_CAPACITY, sizeof(ast_decl),		&decls_list);
	list_init(INITIAL_CAPACITY, sizeof(ast_assign),		&assigns_list);
	list_init(INITIAL_CAPACITY, sizeof(ast_while),		&whiles_list);
	list_init(INITIAL_CAPACITY, sizeof(ast_call),		&calls_list);

	list_init(INITIAL_CAPACITY, sizeof(ast_const),		&consts_list);
	list_init(INITIAL_CAPACITY, sizeof(ast_var),		&vars_list);
	list_init(INITIAL_CAPACITY, sizeof(ast_bin_op),		&bin_ops_list);

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



// COMMON

inline static error_code create_node(
	list* list, size_t node_size, enum ast_type type, ulong line, ulong pos, void** _ptr
)
{
	if (!_ptr) return EX_NULL;
	if (*_ptr) return EX_OUTARG_NOT_NULL;

	error_code e = list_append(list, node_size, _ptr);
	if (e) return e;
	((ast_node*)*_ptr)->type = type;
	((ast_node*)*_ptr)->line = line;
	((ast_node*)*_ptr)->pos = pos;
	return SUCCESS;
}

#define CREATE_NODE(list, type, enum_val) \
	create_node(&list, sizeof(type), enum_val, line, pos, _node);

#define CREATE_NODE_WITH_STRING(list, type, enum_val, str_attr)		\
	error_code e = CREATE_NODE(list, type, enum_val, line, pos);	\
	if (e) return e;												\
	(*_node)->str_attr = str_attr; (*_node)->len = len

inline static error_code append_node(list* list, void* node)
{
	void** node_ptr = NULL;
	error_code e = list_append(list, sizeof(void*), &node_ptr);
	if (e) return e;

	*node_ptr = node;
	return SUCCESS;
}



// SEQUENCE

error_code ast_create_sequence(ast_sequence** _node, ulong line, ulong pos)
{
	return CREATE_NODE(seqs_list, ast_sequence, AST_SEQUENCE);
}

error_code ast_seq_append(ast_sequence* node, ast_stm* stm)
{
	return append_node(&(node->statements), stm);
}



// DECLARATION

error_code ast_create_decl(ast_decl** _node, ulong line, ulong pos, const char* identifier, byte len)
{
	CREATE_NODE_WITH_STRING(decls_list, ast_decl, AST_DECL, identifier);
	return SUCCESS;
}

// ASSIGNMENT

error_code ast_create_assign(ast_assign** _node, ulong line, ulong pos, const char* var, byte len)
{
	CREATE_NODE_WITH_STRING(decls_list, ast_decl, AST_DECL, var);
	return SUCCESS;
}

// WHILE

error_code ast_create_while(ast_while** _node, ulong line, ulong pos)
{
	return CREATE_NODE(whiles_list, ast_while, AST_WHILE);
}

error_code ast_while_append(ast_while* node, ast_stm* stm)
{
	return append_node(&(node->body), stm);
}

// CALL

error_code ast_create_call(ast_call** _node, ulong line, ulong pos, const char* identifier, byte len)
{
	CREATE_NODE_WITH_STRING(calls_list, ast_call, AST_CALL, identifier);
	return SUCCESS;
}

error_code ast_call_append(ast_call* node, ast_expr* arg)
{
	return append_node(&(node->args), arg);
}



// CONSTANT

error_code ast_create_const(ast_const** _node, ulong line, ulong pos, word val)
{
	error_code e = CREATE_NODE(consts_list, ast_const, AST_CONST);
	if (e) return e;
	(*_node)->val = val;
	return SUCCESS;
}

// VARIABLE

error_code ast_create_var(ast_var** _node, ulong line, ulong pos, const char* identifier, byte len)
{
	CREATE_NODE_WITH_STRING(vars_list, ast_var, AST_VAR, identifier);
	return SUCCESS;
}

// BINARY OPERATOR

error_code ast_create_bin_op(ast_bin_op** _node, ulong line, ulong pos, enum ast_op op)
{
	error_code e = CREATE_NODE(bin_ops_list, ast_bin_op, AST_BIN_OP);
	if (e) return e;
	(*_node)->op = op;
	return SUCCESS;
}



// DEBUG

#include <stdio.h>
#include <string.h>

const char* ast_type_to_string(enum ast_type type)
{
	switch (type)
	{
	case AST_NONE:		return "None";

	case AST_SEQUENCE:	return "Sequence";

	case AST_DECL:		return "Declaration";
	case AST_ASSIGN:	return "Assignment";
	case AST_WHILE:		return "While";
	case AST_CALL:		return "Call";

	case AST_CONST:		return "Constant";
	case AST_BIN_OP:	return "Binary Operator";
	case AST_VAR:		return "Variable";

	default:			return "[ERROR: Unknown Type]";
	}
}

inline static void print_indents(uint count) { for (uint i = 0; i < count; i++) printf("  "); }

static void print_node(ast_node* node, uint depth)
{
	print_indents(depth);
	if (!node)
	{
		printf("%s\n", (const char*)NULL);
		return;
	}
	printf("%s [%d:%d]\n", ast_type_to_string(node->type), node->line, node->pos);

	switch (node->type)
	{
	case AST_SEQUENCE:
	{
		ast_sequence* concrete = node;
		for (int i = 0; i < concrete->statements.count; i++)
			print_node(&(LIST_GET(concrete->statements, ast_stm*, i)->node), depth + 1);
		break;
	}

	case AST_DECL:
	{
		ast_decl* concrete = node;
		print_indents(depth + 1);
		printf("Identifier '");
		PRINT_STR(concrete->identifier, concrete->len, 0x100);
		printf("'\n");
		print_node(concrete->val, depth + 1);
		break;
	}
	case AST_ASSIGN:
	{
		ast_assign* concrete = node;
		print_indents(depth + 1);
		printf("Variable '");
		PRINT_STR(concrete->var, concrete->len, 0x100);
		printf("'\n");
		print_node(concrete->val, depth + 1);
		break;
	}
	case AST_WHILE:
	{
		ast_while* concrete = node;
		print_node(concrete->condition, depth + 1);
		for (int i = 0; i < concrete->body.count; i++)
			print_node(&(LIST_GET(concrete->body, ast_stm*, i)->node), depth + 1);
		break;
	}

	case AST_CONST:
	{
		ast_const* concrete = node;
		print_indents(depth + 1);
		printf("Value %d\n", concrete->val);
		break;
	}
	case AST_VAR:
	{
		ast_var* concrete = node;
		print_indents(depth + 1);
		printf("Identifier '");
		PRINT_STR(concrete->identifier, concrete->len, 0x100);
		printf("'\n");
		break;
	}
	case AST_BIN_OP:
	{
		ast_bin_op* concrete = node;
		print_indents(depth + 1);
		printf("Operator %c\n", concrete->op);
		print_node(&(concrete->lhs->node), depth + 1);
		print_node(&(concrete->rhs->node), depth + 1);
		break;
	}
	}
}

error_code ast_print(ast_node* root)
{
	if (!root) return EX_NULL;
	print_node(root, 0);
	return SUCCESS;
}
