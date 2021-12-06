#include "util.h"
#include "except.h"

#include "list.h"

enum ast_type {
	AST_NONE = 0,
	AST_SEQUENCE,
	AST_DECL, AST_ASSIGN, AST_WHILE, AST_CALL,
	AST_CONST, AST_BIN_OP, AST_VAR
};

enum ast_op {
	AST_LT = '<',
	AST_PLUS = '+'
};

// NODES

typedef struct ast_node ast_node;
typedef struct ast_sequence ast_sequence;		// Sequence Node

typedef struct ast_stm ast_stm;					// Statement Node
typedef struct ast_decl ast_decl;				// Declaration Node
typedef struct ast_assign ast_assign;			// Assignment Node
typedef struct ast_while ast_while;				// While Loop Node
typedef struct ast_call ast_call;				// Function Call Node

typedef struct ast_expr ast_expr;				// Expression Node
typedef struct ast_const ast_const;				// Constant Node
typedef struct ast_var ast_var;					// Variable Node
typedef struct ast_bin_op ast_bin_op;			// Binary Operator Node



error_code init_ast();
error_code free_ast();

struct ast_node {
	ulong line, pos;
	enum ast_type type;
};

struct ast_sequence {
	ast_node node;
	list statements; // list<ast_stm*>
};
error_code ast_create_sequence(ast_sequence** _node, ulong line, ulong pos);
error_code ast_seq_append(ast_sequence* node, ast_stm* stm);



struct ast_stm {
	ast_node node;
};

struct ast_decl {
	ast_stm stm;
	const char* identifier; byte len;
	ast_expr* val;
};
error_code ast_create_decl(ast_decl** _node, ulong line, ulong pos, const char* identifier, byte len);

struct ast_assign {
	ast_stm stm;
	const char* var; byte len;
	ast_expr* val;
};
error_code ast_create_assign(ast_assign** _node, ulong line, ulong pos, const char* var, byte len);

struct ast_while {
	ast_stm stm;
	ast_bin_op* condition;
	list body; // list<ast_stm*>
};
error_code ast_create_while(ast_while** _node, ulong line, ulong pos);
error_code ast_while_append(ast_while* node, ast_stm* stm);

struct ast_call {
	ast_stm stm;
	const char* identifier; byte len;
	list args; // list<ast_expr*>
};
error_code ast_create_call(ast_call** _node, ulong line, ulong pos, const char* identifier, byte len);
error_code ast_call_append(ast_call* node, ast_expr* arg);



struct ast_expr {
	ast_node node;
};

struct ast_const {
	ast_expr expr;
	word val;
};
error_code ast_create_const(ast_const** _node, ulong line, ulong pos, word val);

struct ast_var {
	ast_expr expr;
	const char* identifier; byte len;
};
error_code ast_create_var(ast_var** _node, ulong line, ulong pos, const char* identifier, byte len);

struct ast_bin_op {
	ast_expr expr;
	enum ast_op op;
	ast_expr* lhs;
	ast_expr* rhs;
};
error_code ast_create_bin_op(ast_bin_op** _node, ulong line, ulong pos, enum ast_op op);
