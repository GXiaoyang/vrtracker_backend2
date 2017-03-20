#include "log.h"
#include <stdio.h>
#include <cstdarg>
#include <assert.h>

#ifdef _WIN32
#include <Windows.h>
#endif

static bool g_bPrintf = true;
static bool g_log_to_file = false;

static bool file_opened;
static FILE *pf;

void log_printf(const char *fmt, va_list argp) 
{
	vfprintf(stderr, fmt, argp);
}

void log_printf(const char *fmt, ...)
{
	va_list args;
	char buffer[2048];

	va_start(args, fmt);
	vsprintf_s(buffer, fmt, args);
	va_end(args);

	if (g_bPrintf)
		printf("%s", buffer);

	if (g_log_to_file)
	{
		if (!pf)
		{
			fopen_s(&pf, "c:\\vr_streams\\bla.log", "wt");
		}
		if (pf)
		{
			fprintf(pf, "%s", buffer);
			fflush(pf);
		}
	}
#ifdef _WIN32
	OutputDebugStringA(buffer);
#endif
}

void ABORT(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	log_printf(fmt, args);
	va_end(args);

	assert(0);
}
