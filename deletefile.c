// deletefile.c
//
// Time-stamp: <12/02/01 14:41:13 keuchel@keuchelnt>

#include "celib.h"

BOOL XCEAPI
XCEDeleteFileA(const char *fname)
{
  wchar_t wfname[MAX_PATH];

  MultiByteToWideChar(CP_ACP, 0, fname, -1, wfname, COUNTOF(wfname));

  return XCEDeleteFileW(wfname);
}

BOOL XCEAPI
XCEDeleteFileW(const wchar_t *wfname)
{
  wchar_t wpath[MAX_PATH];

  XCEFixPathW(wfname, wpath);

  return DeleteFileW(wpath);
}
