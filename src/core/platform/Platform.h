#pragma once

#include "core/base/Macros.h"

#ifdef _WIN32
#include <winsock.h>
#endif

int gettimeofday(struct timeval * val, struct timezone *);

void Log(const char* format, ...);
