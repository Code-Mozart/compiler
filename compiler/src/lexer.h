#pragma once

#include "util.h"
#include "except.h"
#include "token.h"

error_code lex_analyze(const char* src, ulong size, token** _tokens, ulong* _count);
