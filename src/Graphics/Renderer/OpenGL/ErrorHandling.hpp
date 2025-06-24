#pragma once

#include <cassert>

#ifdef DEBUGGGGGG
#define glCall(x) glClearError();\
x;\
assert(glLogCall(#x));
#else
#define glCall(x) x;
#endif


void glClearError();

bool glLogCall(const char* function);