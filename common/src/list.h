#pragma once

#include "util.h"
#include "except.h"

#define LIST_RESIZE_FN(x) ((x * 3) / 2)

typedef struct list {
	void* ptr; size_t capacity;
	size_t count;
} list;

// 'element_size' in bytes
error_code list_init(size_t initial_capacity, size_t element_size, list* _list);
// returns the pointer to the appended element,
// the caller should cast it and set its values
// 'element_size' in bytes
error_code list_append(list* list, size_t element_size, void* _ptr);
// releases the list, deletes its knowledge of the memory block
// and writes them into the output parameters
// ! CAREFULL, if the ptr is not previously captured the allocated
// memory can never be freed
error_code list_release(list* list, void* _ptr, size_t* _capacity, size_t* _count);
error_code list_free(list* list);


#ifdef IMPL_COMMON

#include <stdlib.h>
#include <string.h>

error_code list_init(size_t initial_capacity, size_t size, list* _list)
{
	if (!_list) return EX_NULL;
	if (!initial_capacity || !size) return EX_INV_ARG;

	_list->count = 0;
	_list->ptr = calloc(initial_capacity, size);
	if (!_list->ptr)
	{
		_list->capacity = 0;
		return EX_MALLOC;
	}
	_list->capacity = initial_capacity;
	return SUCCESS;
}

// trust parameters are valid
inline static error_code list_resize(list* list, size_t size, size_t min_capacity)
{
	size_t new_capacity = min(LIST_RESIZE_FN(list->capacity), min_capacity);
	void* temp = calloc(new_capacity, size);
	if (!temp) return EX_MALLOC;

	memcpy_s(temp, new_capacity, list->ptr, list->capacity);
	free(list->ptr);

	list->ptr = temp;
	list->capacity = new_capacity;
	return SUCCESS;
}

error_code list_append(list* list, size_t element_size, void** _ptr)
{
	if (!list || !_ptr) return EX_NULL;
	if (*_ptr) return EX_OUTARG_NOT_NULL;

	if (list->count >= list->capacity)
	{
		error_code e = list_resize(list, element_size, list->capacity + 1);
		if (e) return e;
	}

	*_ptr = (byte*)list->ptr + (element_size * list->count++);
	return SUCCESS;
}

error_code list_release(list* list, void** _ptr, size_t* _capacity, size_t* _count)
{
	if (!list) return EX_NULL;
	if (_ptr) *_ptr = list->ptr;
	if (_capacity) *_capacity = list->capacity;
	if (_count) *_count = list->count;

	list->ptr = NULL;
	list->capacity = 0;
	list->count = 0;
	return SUCCESS;
}

error_code list_free(list* list)
{
	if (!list) return EX_NULL;
	free(list->ptr);
	list->capacity = 0;
	list->count = 0;
	return SUCCESS;
}

#endif
