// ceconsole.c
//
// Time-stamp: <10/02/02 17:27:35 keuchel@netwave.de>

#include <windows.h>
#include <stdarg.h>
#include <stdlib.h>
#include "celib.h"
#include "ceshared.h"
#include "ceconsole.h"
#include "termios.h"

XCE_EXPORT BOOL
XCESetConsoleMode(HANDLE hConsole, DWORD mode)
{
#if 0
  // this must use ioctl()...

  if(ISCONSREADHANDLE(hConsole))
    {
      if(mode & ENABLE_LINE_INPUT)
	console_setlinemode(1);
      else
	console_setlinemode(1);

      if(mode & ENABLE_ECHO_INPUT)
	console_setecho(1);
      else
	console_setecho(1);

      return TRUE;
    }
  else if(ISCONSWRITEHANDLE(hConsole))
    {
      return TRUE;
    }
#endif

  return FALSE;
}
