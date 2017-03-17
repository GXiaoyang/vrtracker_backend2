#include "log.h"
#include <stdio.h>
#include <cstdarg>
#include <Windows.h>

static bool g_bPrintf = true;
static bool g_log_to_file = true;

static bool file_opened;
static FILE *pf;

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

	OutputDebugStringA(buffer);
}