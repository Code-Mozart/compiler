#pragma once

#include "except.h"
#include "util.h"

error_code load_file(const char* const path, char** _content, ulong* _size);

#ifdef IMPL_COMMON

#include <stdio.h>
#include <stdlib.h>

error_code load_file(const char* const path, char** _content, ulong* _size)
{
	if (!path || !_size || !_content) return EX_NULL;
	if (*_content) return EX_OUTARG_NOT_NULL;

	FILE* file;
	errno_t err = fopen_s(&file, path, "rb");
	if (err || !file)
	{
		// todo: write proper error
		return EX_FILE;
	}

	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	if (file_size == -1L)
	{
		// todo: write proper error
		fclose(file);
		return EX_FILE;
	}
	else
	{
		*_size = file_size;
	}

	ulong buffer_size = sizeof(char) * (file_size + 1L);
	char* buffer = (char*)malloc(buffer_size);
	if (!buffer)
	{
		fclose(file);
		return EX_MALLOC;
	}

	rewind(file);
	uint read_size = fread_s(buffer, buffer_size, sizeof(char), file_size, file);
	if (read_size != file_size)
	{
		free(buffer);
		buffer = NULL;
		_size = 0;
		fclose(file);
		return EX_FILE;
	}
	buffer[buffer_size - 1] = '\0';
	*_content = buffer;

	fclose(file);
	return SUCCESS;
}

#endif
