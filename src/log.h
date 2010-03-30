#ifndef LOG_H_
#define LOG_H_

#include <stdio.h>
#include <string.h>

#ifdef DEBUG
#define debug(x, ...) debug_##x(__VA_ARGS__)
#define debug_real(_fmt, ...) fprintf(stderr, "\033[31m%s:%d \033[0m\033[34m%s()\033[0m: "_fmt"\n", strrchr(__FILE__, '/'), __LINE__, __FUNCTION__, ##__VA_ARGS__)
#else
#define debug_real(_fmt, ...)
#define debug(x, ...)
#endif

#if DEBUG >= 1
#define debug_1(_fmt, ...) debug_real(_fmt, ##__VA_ARGS__)
#else
#define debug_1(_fmt, ...)
#endif

#if DEBUG >= 2
#define debug_2(_fmt, ...) debug_real(_fmt, ##__VA_ARGS__)
#else
#define debug_2(_fmt, ...)
#endif

#if DEBUG >= 3
#define debug_3(_fmt, ...) debug_real(_fmt, ##__VA_ARGS__)
#else
#define debug_3(_fmt, ...)
#endif

#endif //LOG_H_
