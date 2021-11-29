#include "stack.h"

void init_stack()
{
	stack.ptr = &stack.buffer;
}


bool is_stack_empty()
{
	return stack.ptr == &stack.buffer;
}


error_code stack_push(word w)
{
	if (stack.ptr < &stack.buffer + STACK_SIZE)
	{
		*stack.ptr = w;
		++stack.ptr;
		return SUCCESS;
	}
	else
	{
		return EX_FULL;
	}
}

error_code stack_pop(word* _w)
{
	if (is_stack_empty())
	{
		return EX_EMPTY;
	}
	else
	{
		--stack.ptr;
		if (_w)
			*_w = *stack.ptr;
		*stack.ptr = 0;
		return SUCCESS;
	}
}

error_code stack_read(ushort offset, word* _w)
{
	if (!_w) return EX_NULL;

	word* ptr = (stack.ptr - 1) - offset;
	if (ptr >= &stack.buffer)
	{
		*_w = *ptr;
		return SUCCESS;
	}
	else
	{
		return EX_INDEX;
	}
}

error_code stack_write(ushort offset, word w)
{
	word* ptr = (stack.ptr - 1) - offset;
	if (ptr >= &stack.buffer)
	{
		*ptr = w;
		return SUCCESS;
	}
	else
	{
		return EX_INDEX;
	}
}
