#pragma once

#include "except.h"
#include "instruction.h"

#define EX_LOAD_INSTRUCTIONS	((error_code) 0x8000)
#define EX_UNKNOWN_OPCODE		((error_code) 0x8001)
#define EX_ILLEGAL_CHAR			((error_code) 0x8002)
#define EX_MAX_INSTRUCTIONS		((error_code) 0x8003)

struct
{
	ulong line;
	char opcode[0xFF];
} loader_except;

error_code load_bytecode(const char* const path, instruction** _instructions, ulong* _count);
