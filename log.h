#pragma once

void log_printf(const char *fmt, ...);

inline void log_entry(const char *fn)
{
	log_printf("> %s\n", fn);
}

inline void log_entry(const char *fn, const char *param)
{
	log_printf("> %s %s\n", fn, param);
}

inline void log_entry(const char *fn, int d)
{
	log_printf("> %s %d\n", fn, d);
}

template <typename T>
inline void log_exit_rc(const char *fn, const T &ret)
{
	using namespace std;
	using namespace openvr_string;
	std::string s = to_string(ret);
	log_printf("< %s\n\n", s.c_str());
}

#define NO_LOG
#ifdef NO_LOG
#define LOG_ENTRY(x)
#define LOG_ENTRY_1(x,y)
#define LOG_EXIT(x)  
#define LOG_EXIT_RC(x,y) return x
#else
#define LOG_ENTRY(x)		log_entry(x);
#define LOG_ENTRY_1(x,y)	log_entry(x,y);
#define LOG_EXIT(x)  log_printf("< %s\n\n", x);
#define LOG_EXIT_RC(x,y) log_exit_rc(y,x); return x
#endif