// ceassert.c
//
// Time-stamp: <13/04/01 14:28:31 keuchel@w2k>

#include "celib.h"
#include "assert.h"

int
xcehandleassert(const char *fmt, ...)
{
  va_list ap;
  char buf[512];
  wchar_t wbuf[512];

  va_start(ap, fmt);
  vsprintf(buf, fmt, ap);
  va_end(ap);

  MultiByteToWideChar(CP_ACP, 0, buf, -1, wbuf, COUNTOF(wbuf));

  MessageBoxW(NULL, wbuf, _T("Assert"), MB_OK);
  DebugBreak();

  return 0;
}
