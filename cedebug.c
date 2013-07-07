// cedebug.c
//
// Time-stamp: <04/04/02 08:42:36 keuchel@netwave.de>

#include "celib.h"

int __xcetrace = 0;
HANDLE __xcetracefile;

void
XCETrace(const char *fmt, ...)
{
  char buf[1024];
  wchar_t wbuf[1024];
  int len;

  va_list ap;

  if(!__xcetrace)
    return;

#if 1
  len = sprintf(buf, "T%08x: ", GetCurrentThreadId());
#else
  len = sprintf(buf, "%08d:%08x: ", GetTickCount(), GetCurrentProcessId());
#endif

  va_start(ap, fmt);
  vsprintf(buf + len, fmt, ap);
  strcat(buf, "\r\n");

  if(__xcetracefile)
    {
      DWORD dwWritten;

      SetFilePointer(__xcetracefile, 0, NULL, FILE_END);
      WriteFile(__xcetracefile, buf, strlen(buf), &dwWritten, NULL);
      FlushFileBuffers(__xcetracefile);
    }
  else
    {
      MultiByteToWideChar(CP_ACP, 0, buf, -1, wbuf, 1024);
      OutputDebugStringW(wbuf);
    }
}

void
XCETraceW(const wchar_t *fmt, ...)
{
  wchar_t buf[1024];
  va_list ap;

  if(!__xcetrace)
    return;

  va_start(ap, fmt);
  vswprintf(buf, fmt, ap);

  wcscat(buf, L"\r\n");

  OutputDebugStringW(buf);
}

#undef CreateFileW

void
XCEOpenTraceFile(char *fname)
{
  wchar_t wfname[MAX_PATH];

  MultiByteToWideChar(CP_ACP, 0, fname, -1, wfname, COUNTOF(wfname));

  __xcetracefile = CreateFileW(wfname, GENERIC_WRITE, 
			       FILE_SHARE_WRITE,
			       NULL, OPEN_ALWAYS,
			       0, NULL);

  if(__xcetracefile == (HANDLE) -1)
    {
      __xcetracefile = 0;
      return;
    }

  SetFilePointer(__xcetracefile, 0, NULL, FILE_END);
}
