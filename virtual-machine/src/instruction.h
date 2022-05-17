#pragma once

#include <stddef.h>

#include "util.h"

#define MAX_OPCODE_LEN 4

enum opcode {
	NOP, STOP,	
	PUSH, POP, LOAD, STR,
	JMP, JZ,
	CALL, RET,
	ADD, SUB, MUL, DIV, MOD,
	LT,
	COUT
};

const char* opcode_strings[];
size_t get_opcodes_count();
#define OPCODE_STRINGS const char* opcode_strings[] = {				\
							"nop", "stop",							\
							"push", "pop", "load", "str",			\
							"jmp", "jz",							\
							"call", "ret",							\
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
