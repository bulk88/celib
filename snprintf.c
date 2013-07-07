// snprintf.c
//
// Time-stamp: <03/01/02 22:21:30 keuchel@netwave.de>

#include <stdarg.h>
#if UNDER_CE > 201
#include <stdio.h>
#endif
#include "celib.h"

XCE_EXPORT int 
xcesnprintf(char *buf, size_t count, const char *fmt, ...)
{
  va_list ap;
  int n = 0;

  va_start(ap, fmt);
  n = _vsnprintf(buf, count, fmt, ap);
  va_end(ap);
  return n;
}
