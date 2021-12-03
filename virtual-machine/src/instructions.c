#include "instruction.h"

OPCODE_STRINGS;
size_t get_opcodes_count() { return ARRLEN(opcode_strings); }
