// fileattr.c
//
// Time-stamp: <12/02/01 14:40:59 keuchel@keuchelnt>

#include "celib.h"

DWORD XCEAPI
XCEGetFileAttributesA(const char *fname)
{
  wchar_t wfname[MAX_PATH];
  DWORD dwRes;

  MultiByteToWideChar(CP_ACP, 0, fname, -1, wfname, COUNTOF(wfname));

  dwRes = XCEGetFileAttributesW(wfname);

  return dwRes;
}

BOOL XCEAPI
XCESetFileAttributesA(const char *fname, DWORD dwAttr)
{
  wchar_t wfname[MAX_PATH];
  BOOL res;

  MultiByteToWideChar(CP_ACP, 0, fname, -1, wfname, COUNTOF(wfname));

  res = XCESetFileAttributesW(wfname, dwAttr);

  return res;
}

DWORD XCEAPI
XCEGetFileAttributesW(const wchar_t *wfname)
{
  wchar_t wpath[MAX_PATH];
  DWORD dwRes;

  XCEFixPathW(wfname, wpath);
  dwRes = GetFileAttributesW(wpath);

  return dwRes;
}

BOOL XCEAPI
XCESetFileAttributesW(const wchar_t *wfname, DWORD dwAttr)
{
  wchar_t wpath[MAX_PATH];
  BOOL res;

  XCEFixPathW(wfname, wpath);
  res = SetFileAttributesW(wpath, dwAttr);

  return res;
}
