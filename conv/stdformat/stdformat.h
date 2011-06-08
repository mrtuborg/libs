#ifndef STDFORMAT_H
#define STDFORMAT_H

#include <string>
int format_arg_list(std::string& out, int length, const char *fmt, va_list args);
std::string format(const char *fmt, ...);

#endif
