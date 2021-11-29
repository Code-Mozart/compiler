#include "loader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileio.h"

static error_code get_opcode(char* buffer, enum opcode* _opcode)
{
	if (!buffer || !_opcode) return EX_NULL;

	for (int i = 0; i < get_opcodes_count(); i++)
	{
		if (strcmp(buffer, opcode_strings[i]) == 0)
		{
			*_opcode = (enum opcode)i;
			return SUCCESS;
		}
	}
	return EX_NOT_FOUND;
}

static error_code cancel(
	char* content_buffer,
	instruction* instructions_buffer,
	ulong line,
	char* opcode_buffer,
	byte opcode_len,
	error_code e
)
{
	free(content_buffer);
	free(instructions_buffer);

	loader_except.line = line;
	if (opcode_buffer)
		strncpy_s(loader_except.opcode, ARRLEN(loader_except.opcode), opcode_buffer, opcode_len + 1);
	else
		memset(loader_except.opcode, '\0', ARRLEN(loader_except.opcode));

	return e;
}

error_code load_bytecode(const char* const path, instruction** _instructions, ulong* _count)
{
	if (!path || !_instructions || !_count) return EX_NULL;
	if (*_instructions) return EX_OUTARG_NOT_NULL;

	char* content = NULL;
	ulong size = 0;
	error_code e = load_file(path, &content, &size);
	if (e) return e;

	ulong max_instructions = 1;
	for (int i = 0; i < size; i++)
		if (content[i] == '\n') max_instructions++;
	instruction* buffer = (instruction*)malloc(sizeof(instruction) * max_instructions);
	if (!buffer)
	{
		free(content);
		return EX_MALLOC;
	}

	ulong line = 1;
	ulong count = 0;
	char opcode_buffer[MAX_OPCODE_LEN + 1]; byte opcode_len = 0;
	memset(opcode_buffer, '\0', sizeof(opcode_buffer));
	word param = 0;
	enum { comment = 0x80, negative = 0x40 };
	byte state = 0x0;
	for (int i = 0; i < size; i++)
	{
		char c = content[i];
		if ((state & comment) && c != ';')
		{
			if (c == '\n') line++;
			continue;
		}
		switch (c)
		{
		case ';':
			state ^= comment;
			break;
		case '\n':
		{
			if (!opcode_len)
			{
				line++;
				break;
			}

			enum opcode opcode;
			if (get_opcode(opcode_buffer, &opcode))
				return cancel(content, buffer, line, opcode_buffer, opcode_len, EX_UNKNOWN_OPCODE);

			if (state & negative)
				param = -param;
			buffer[count] = (instruction){ opcode, 0, param };
			count++;
			if (count > max_instructions)
				return cancel(content, buffer, line, opcode_buffer, opcode_len, EX_MAX_INSTRUCTIONS);

			memset(opcode_buffer, '\0', sizeof(opcode_buffer));
			opcode_len = 0;
			param = 0;
			state &= comment;
			line++;
			break;
		}
		case ' ':
			state++;
			break;
		default:
			if (!((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '-'))
				break;

			if ((state & 0xF) == 0)
			{
				if (opcode_len > MAX_OPCODE_LEN)
					return cancel(content, buffer, line, NULL, 0, EX_UNKNOWN_OPCODE);
				opcode_buffer[opcode_len++] = c;
			}
			else if ((state & 0xF) == 1)
			{
				if (c == '-' && !param)
				{
					state |= negative;
					break;
				}

				if (!(c >= '0' && c <= '9'))
					return cancel(content, buffer, line, opcode_buffer, opcode_len, EX_ILLEGAL_CHAR);
				param = param * 10 + (c - '0');
			}
		}
	}

	*_instructions = buffer;
	*_count = count;
	free(content);
	return SUCCESS;
}
