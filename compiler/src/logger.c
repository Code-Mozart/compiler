#include "logger.h"

#include <stdio.h>

#include "except.h"

void cm_log(enum severity severity, const char* msg)
{
	PRINT_ERR("not implemented");
}

log_entry pop_log_entry()
{
	PRINT_ERR("not implemented");
	return (log_entry){ 0, 0 };
}
