#include <stdio.h>

#include "processor.h"

// EXECUTE

void run(instruction* instructions, ulong count)
{
	if (!instructions)
	{
		printf("!! error\n");
		return;
	}
	void* max_pc = instructions + count;

	init_stack();

	uint cycles = 0;
	uint line = 1;
	instruction* pc = instructions;
	bool inc_pc;

	while (pc)
	{
		inc_pc = true;

		if (pc >= max_pc)
		{
			printf("!! pc overflow\n");
			return;
		}

		switch (pc->opcode)
		{
		case PUSH:
			stack_push(pc->param);
			break;
		case POP:
			stack_pop(NULL);
			break;
		case LOAD:
		{
			word w = 0;
			stack_read(pc->param, &w);
			stack_push(w);
			break;
		}
		case STR:
		{
			word w = 0;
			stack_read(0, &w);
			stack_write(pc->param, w);
			stack_pop(NULL);
			break;
		}

		case JMP:
		{
			line += pc->param;
			pc += pc->param;
			inc_pc = false;
			break;
		}
		case JZ:
		{
			word w = 0;
			stack_pop(&w);
			if (!w)
			{
				line += pc->param;
				pc += pc->param;
				inc_pc = false;
			}
			break;
		}

		case CALL:
		{
			word call_line = pc - instructions;
			stack_push(call_line);
			line = call_line + 1;
			pc = instructions + pc->param;
			inc_pc = false;
			break;
		}
		case RET:
		{
			word w = 0;
			stack_pop(&w);
			line = w + 1;
			pc = instructions + w;
			break;
		}

		case ADD:
		{
			word s1 = 0, s2 = 0;
			stack_pop(&s1);
			stack_pop(&s2);
			stack_push(s1 + s2);
			break;
		}
		case SUB:
		{
			word minuend = 0, subtrahend = 0;
			stack_pop(&subtrahend);
			stack_pop(&minuend);
			stack_push(minuend - subtrahend);
			break;
		}
		case MUL:
		{
			word f1 = 0, f2 = 0;
			stack_pop(&f1);
			stack_pop(&f2);
			stack_push(f1 * f2);
			break;
		}
		case DIV:
		{
			word dividend = 0, divisor = 0;
			stack_pop(&divisor);
			if (!divisor)
			{
				printf("!! division by zero");
				return;
			}
			stack_pop(&dividend);
			stack_push(dividend / divisor);
			break;
		}
		case MOD:
		{
			word dividend = 0, divisor = 0;
			stack_pop(&divisor);
			if (!divisor)
			{
				printf("!! division by zero");
				return;
			}
			stack_pop(&dividend);
			stack_push(dividend % divisor);
			break;
		}

		case LT:
		{
			word lhs = 0, rhs = 0;
			stack_pop(&rhs);
			stack_pop(&lhs);
			stack_push(lhs < rhs);
			break;
		}

		case COUT:
		{
			word w = 0;
			stack_pop(&w);
			printf("[from line %d] >> %d\n", line, w);
			break;
		}

		case STOP:
			pc = NULL;
			inc_pc = false;
			break;
		}

		//printf("%d| %s %d:", line, opcode_strings[instructions[line - 1].opcode], instructions[line - 1].param);
		//print_stack();

		cycles++;
		if (inc_pc)
		{
			line++;
			++pc;
		}
	}

	printf("programm finished after %d cycles\n", cycles);
}

// DEBUG

void print_stack()
{
	printf("\n------------- STACK ---------------\n");

	if (is_stack_empty())
	{
		printf("empty\n");
	}
	else
	{
		uint pos = stack.ptr - &stack.buffer - 1;
		word* ptr = stack.ptr - 1;
		while (true)
		{
			printf("%d\t$%p\t%d\n", pos, ptr, *ptr);

			if (pos)
			{
				--pos;
				--ptr;
			}
			else break;
		}
	}

	printf("-----------------------------------\n\n");
}
