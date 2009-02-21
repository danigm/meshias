#ifndef LOG_H_
#define LOG_H_


#define DEBUG 2

#ifdef DEBUG

void debug_real(int x, char* str);

#define debug(x, y) debug_##x(y)

#if DEBUG >= 1
#define debug_1(y) debug_real(1, y)
#else
#define debug_1(y)
#endif // 1

#if DEBUG >= 2
#define debug_2(y) debug_real(2, y)
#else
#define debug_2(y)
#endif // 2

#if DEBUG >= 3
#define debug_3(y) debug_real(3, y)
#else
#define debug_3(y)
#endif // 3

#else
#define debug(...)
#endif

#endif //LOG_H_
