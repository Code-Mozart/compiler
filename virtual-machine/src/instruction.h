#pragma once

#include "util.h"

#define MAX_OPCODE_LEN 4

enum opcode {
	NOP, STOP,	
	PUSH, LOAD, STR,
	JMP, JZ,
	ADD, SUB, MUL, DIV, MOD,
	LT,
	COUT
};

const char* opcode_strings[];
uint get_opcodes_count();
#define OPCODE_STRINGS const char* opcode_strings[] = {				\
							"nop", "stop",							\
							"push", "load", "str",					\
							"jmp", "jz",							\
							"add", "sub", "mul", "div", "mod",		\
							"lt",									\
							"cout"									\
							}

typedef struct instruction
{
	enum opcode opcode;
	byte padding;
	word param;
} instruction;
