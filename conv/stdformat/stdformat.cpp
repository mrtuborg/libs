#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <string>


// for unicode use std::basic_string<wchar_t> instead std::string

int format_arg_list(std::string& out, int length, const char *fmt, va_list args)
{
    if (!fmt) return -1;
    int   result = 0;
    char *buffer = 0;


    buffer = new char [length + 1];
    memset(buffer, 0, length + 1);
    result = vsnprintf(buffer, length, fmt, args);


    out = buffer;  // ~ assign

    delete [] buffer;
    return result;
}

std::string format(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);


    std::string s;
    int length = 256;
    int result = format_arg_list(s,length,fmt, args);
    va_end(args);
    if(result >= 256)
    {
        va_start(args, fmt);
        format_arg_list(s,result+1,fmt, args);
        va_end(args);
    }
    return s;
}



