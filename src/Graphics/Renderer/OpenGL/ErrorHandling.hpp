#pragma once

#include <cassert>

#ifdef NDEBUG
#define glCall(x) x;
#else
#define glCall(x) glClearError();\
x;\
assert(glLogCall(#x));
#endif


void glClearError();

bool glLogCall(const char* function);