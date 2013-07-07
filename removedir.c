// removedir.c
//
// Time-stamp: <12/02/01 14:43:12 keuchel@keuchelnt>

#include "celib.h"

BOOL XCEAPI
XCERemoveDirectoryA(const char *lpName)
{
  wchar_t *lpNameNew = NULL;
  int len = strlen(lpName) + 1;
  BOOL res;

  lpNameNew = malloc(len * 2);
  MultiByteToWideChar(CP_ACP, 0, lpName, -1, lpNameNew, len);

  res = XCERemoveDirectoryW(lpNameNew);

  free(lpNameNew);

  return res;
}

BOOL XCEAPI
XCERemoveDirectoryW(const wchar_t *oldpath)
{
  wchar_t newpath[MAX_PATH];

  XCEFixPathW(oldpath, newpath);
  return RemoveDirectoryW(newpath);
}
