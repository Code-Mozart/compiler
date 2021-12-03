#include "util.h"

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



struct ast_node {
	long line, pos;
	enum ast_type type;
};

struct ast_sequence {
	ast_node node;
	ast_stm* statements; ushort count;
};



struct ast_stm {
	ast_node node;
};

struct ast_decl {
	ast_stm stm;
	const char* identifier; byte len;
	ast_expr* val;
};

struct ast_assign {
	ast_stm stm;
	const char* var; byte len;
	ast_expr* val;
};

struct ast_while {
	ast_stm stm;
	ast_bin_op* condition;
	ast_stm* body; ushort count;
};

struct ast_call {
	ast_stm stm;
	const char* identifier; byte len;
	ast_expr* args; ushort count;
};



struct ast_expr {
	ast_node node;
};

struct ast_const {
	ast_expr expr;
	word val;
};

struct ast_var {
	ast_expr expr;
	const char* identifier; byte len;
};

struct ast_bin_op {
	ast_expr expr;
	enum ast_op op;
	ast_expr* lhs;
	ast_expr* rhs;
};
