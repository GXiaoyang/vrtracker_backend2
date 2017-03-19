#pragma once

void log_printf(const char *fmt, ...);

void ABORT(const char *fmt, ...);

#define LOG_ENTRY(x)
#define LOG_ENTRY_1(x,y)
#define LOG_EXIT(x)  
#define LOG_EXIT_RC(x,y) return x
