#pragma once

enum severity { INFO, WARNING, ERROR };

typedef struct
{
	enum severity severity;
	const char* msg;
} log_entry;

void cm_log(enum severity severity, const char* msg);
log_entry pop_log_entry();
