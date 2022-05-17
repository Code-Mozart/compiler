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
// in case of failure '_ptr' is guaranteed to be null
error_code list_append(list* list, size_t element_size, void** _ptr);

// drops the last element of the list
// 'element_size' in bytes
error_code list_drop(list* list, size_t element_size);

// clears the entire list
// 'element_size' in bytes
error_code list_clear(list* list, size_t element_size);

// releases the list, deletes its knowledge of the memory block
// and writes them into the output parameters
// ! CAREFULL, if the ptr is not previously captured the allocated
// memory can never be freed
error_code list_release(list* list, void** _ptr, size_t* _capacity, size_t* _count);

error_code list_free(list* list);

#define LIST_MAKE(name, type, init_capacity)				\
	list name;												\
	TRY(list_init(init_capacity, sizeof(type), &name));		

#define LIST_GET(list, type, index) (((type*)list.ptr)[index])

#define LIST_GET_LAST(list, type) LIST_GET(list, type, list.count - 1)

#define LIST_APPEND2(list, list_type, val, val_type) \
	{ val_type* ptr = NULL; TRY(list_append(&list, sizeof(list_type), &ptr)); *ptr = val; }
#define LIST_APPEND(list, type, val) LIST_APPEND2(list, type, val, type)

#define LIST_POP(target, list, type) { target = LIST_GET_LAST(list, type); TRY(list_drop(&list, sizeof(type))); }

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
	size_t new_capacity = max(LIST_RESIZE_FN(list->capacity), min_capacity);
	void* temp = calloc(new_capacity, size);
	if (!temp) return EX_MALLOC;

	memcpy_s(temp, new_capacity * size, list->ptr, list->count * size);
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
		TRY(list_resize(list, element_size, list->capacity + 1));

	*_ptr = (byte*)list->ptr + (element_size * list->count++);
	return SUCCESS;
}

error_code list_drop(list* list, size_t element_size)
{
	if (!list) return EX_NULL;
	memset((byte*)list->ptr + (element_size * --(list->count)), 0, element_size);
	return SUCCESS;
}

error_code list_clear(list* list, size_t element_size)
{
	if (!list) return EX_NULL;
	memset(list->ptr, 0, list->capacity * element_size);
	list->count = 0;
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
	list->ptr = NULL;
	list->capacity = 0;
	list->count = 0;
	return SUCCESS;
}

#endif
