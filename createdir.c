// createdir.c
//
// Time-stamp: <12/02/01 14:42:54 keuchel@keuchelnt>

#include "celib.h"

BOOL XCEAPI
XCECreateDirectoryA(const char *lpName, 
		    LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
  wchar_t lpNameNew[MAX_PATH];
  BOOL res;

  MultiByteToWideChar(CP_ACP, 0, lpName, -1, lpNameNew, COUNTOF(lpNameNew));

  res = XCECreateDirectoryW(lpNameNew, lpSecurityAttributes);

  return res;
}

BOOL XCEAPI
XCECreateDirectoryW(const wchar_t *oldpath, LPSECURITY_ATTRIBUTES lpSec)
{
  wchar_t newpath[MAX_PATH];

  XCEFixPathW(oldpath, newpath);
  return CreateDirectoryW(newpath, lpSec);
}
