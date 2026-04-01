#include <stdio.h>
#include <stdarg.h>

void con_init()
{
}

void con_shutdown()
{
}

void con_defer()
{
}

void con_Printf(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

bool con_Input(char* buf, int buflen)
{
    return false;
}
