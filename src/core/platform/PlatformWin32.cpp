#include "core/platform/Platform.h"

#ifdef _WIN32
#include <winnt.h>
#include <profileapi.h>
#include <stdarg.h>
#include <stdio.h>

int gettimeofday(struct timeval * val, struct timezone *) {
	if (val) {
		LARGE_INTEGER liTime, liFreq;
		QueryPerformanceFrequency(&liFreq);
		QueryPerformanceCounter(&liTime);
		val->tv_sec = (long)(liTime.QuadPart / liFreq.QuadPart);
		val->tv_usec = (long)(liTime.QuadPart * 1000000.0 / liFreq.QuadPart - val->tv_sec * 1000000.0);
	}
	return 0;
}

void Log(const char* format, ...) {
	char buf[102400];

	va_list args;
	va_start(args, format);
	vsnprintf(buf, 102400, format, args);
	va_end(args);

	printf("%s\n", buf);
}

#endif