#pragma once

#include "except.h"
#include "instruction.h"

#define STACK_SIZE ((unsigned short) 0x1000)

struct
{
	word buffer[STACK_SIZE];
	word* ptr;
} stack;

void init_stack();

bool is_stack_empty();

error_code stack_push(word w);
error_code stack_pop(word* _w);

error_code stack_read(ushort offset, word* _w);
error_code stack_write(ushort offset, word w);
